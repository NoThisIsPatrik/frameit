#include <iostream>
#include <pqxx/pqxx> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <jpeglib.h>

using namespace std;
using namespace pqxx;

char *mkmedians (const unsigned char*,size_t, int, int,const char*,const char*,int);

int main(int argc, char* argv[]) {
   char *meds;
   try {
      connection C("dbname = framedb");
      connection WC("dbname = framedb");

      if (!C.is_open() || !WC.is_open()) {
         cout << "Can't open database" << endl;
         return 1;
      }
      WC.prepare( "csvins", "INSERT INTO csv_lines( line_id, file_name, csv_line) VALUES($1, $2, $3);");
      work W(WC);

      int n;
      nontransaction N(C);
      result c = N.exec( "SELECT * from frames" ); 
      for (n=0;n<c.size();n++) {
        uint32_t frame_id,cell_x,cell_y;
        string filename;
        string img_dat;
        string ftime;

        frame_id = c[n][0].as<int>();
        filename = c[n][1].as<string>();
        cell_x = c[n][2].as<int>();
        cell_y = c[n][3].as<int>();
        ftime = c[n][4].as<string>();
        
        pqxx:binarystring blob(c[n]["frame_data"]);

        meds = mkmedians(blob.data(),blob.size(),cell_x,cell_y,ftime.c_str(),filename.c_str(),frame_id);
        if(!meds) { // Invalid frame
            continue;
        }
        
        pqxx::result wr = W.prepared("csvins")(frame_id)(filename)(meds).exec();
        cout << "(stored line" <<frame_id<<endl;
        free(meds);
      }
      W.commit();
      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }
}

int comp( const void *a, const void *b) {
    return *(char*)a - *(char*)b;
}

char *mkmedians (const unsigned char*jpg_buffer,size_t jpg_size, int cx, int cy, const char* ftime, const char* fname, int frame_id) {
	int rc, i, j;

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	unsigned long bmp_size;
	unsigned char *bmp_buffer;
	int row_stride, width, height, pixel_size;

	cinfo.err = jpeg_std_error(&jerr);	
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);
    
	rc = jpeg_read_header(&cinfo, TRUE);

	if (rc != 1) return 0; // Not a jpeg

	jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;  height = cinfo.output_height;  pixel_size = cinfo.output_components;

	bmp_size = width * height * pixel_size;  bmp_buffer = (unsigned char*) malloc(bmp_size);
	row_stride = width * pixel_size;

	while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *buffer_array[1];
		buffer_array[0] = bmp_buffer + (cinfo.output_scanline) * row_stride;
		jpeg_read_scanlines(&cinfo, buffer_array, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

    for(j=0;j<height;j++) {
        for(i=0;i<width;i++) {
            int sq,pos,t;

            pos = j*row_stride + i*3 ;
            t = (bmp_buffer[ pos ] + bmp_buffer[ pos+1 ] + bmp_buffer[ pos+2 ])/3;  
            bmp_buffer[ pos ] = t;
            bmp_buffer[ pos+1 ] = t;
            bmp_buffer[ pos+2 ] = t;
            
        }
    }
    unsigned char *med_buf;
    char*string_buf;
    unsigned char *cell_medians;
    int cmp,cmp_size;
    int med_pos,median;
    med_buf = (unsigned char*)malloc(cx*cy);
    cmp_size = ((width/cx)+1) * ((height/cy)+1);

    cell_medians = (unsigned char*)malloc(cmp_size);
    cmp = 0;
    for(j=0;j<height;j+=cy) {
        for(i=0;i<width;i+=cx) {
            int sq,pos,npos,t,ox,oy;
            med_pos = 0;
            for(oy=0;oy<cy;oy++) {
                for(ox=0;ox<cx;ox++) {
                    if(j+oy >= height)
                        continue;
                    if(i+ox >= width)
                        continue;

                    pos = ((j+oy)*row_stride) + ((i+ox)*3) ;
                    med_buf[med_pos++] = bmp_buffer[pos];
                    }
                }
            qsort(med_buf,med_pos,1,comp);
            if(med_pos%2==1) {
                median = med_buf[med_pos/2];
            } else {
                median = (med_buf[med_pos/2] + med_buf[med_pos/2])/2;
            }
            cell_medians[cmp++] = median;

        }
    }
    free(med_buf);
    string_buf = (char*)malloc(cmp*4+100);
    med_pos = 0; 

    // rc = sprintf(string_buf, "INSERT INTO csv_lines( line_id, file_name, csv_line) VALUES (%d, \"%s\", \"%s", frame_id, fname, ftime );
    rc = sprintf(string_buf, "%s", ftime );
    med_pos += rc;
    for(i=0;i<cmp;i++) {
        rc = sprintf(string_buf+med_pos, ",%d", cell_medians[i]);
        med_pos += rc;
    }
    string_buf[med_pos++] = '\0';

	free(bmp_buffer); // The decompressed image
    return(string_buf);
}
