#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

#include <jpeglib.h>

int comp( const void *a, const void *b) {
    return *(char*)a - *(char*)b;
}

int main () {
	int rc, i, j;

	struct stat file_info;
	unsigned long jpg_size;
	unsigned char *jpg_buffer;

        // Read debug file
	stat("test.jpg", &file_info);
	jpg_size = file_info.st_size;
	jpg_buffer = (unsigned char*) malloc(jpg_size + 100);
	int fd = open("test.jpg", O_RDONLY);
	i = 0;
	while (i < jpg_size) {
		rc = read(fd, jpg_buffer + i, jpg_size - i);
		i += rc; }
	close(fd);


	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	unsigned long bmp_size;
	unsigned char *bmp_buffer;
    int cx,cy;
    cx = 256; cy = 256;
	int row_stride, width, height, pixel_size;

        // Start decoding propper
	cinfo.err = jpeg_std_error(&jerr);	
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);
    
	rc = jpeg_read_header(&cinfo, TRUE);

	if (rc != 1) return 1; // Not a jpeg

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
        // Can do another jpg w/o realloc here
	jpeg_destroy_decompress(&cinfo);
        // Not here
	free(jpg_buffer); // This is the are holding the compressed data

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

    rc = sprintf(string_buf, "%.2f", 0.0);
    med_pos += rc;
    for(i=0;i<cmp;i++) {
        rc = sprintf(string_buf+med_pos, ",%d", cell_medians[i]);
        med_pos += rc;
    }
    string_buf[med_pos++] = '\n';
    string_buf[med_pos++] = '\0';

    puts(string_buf);
    free(string_buf);
            // sq = (i/cx) + (j/cy) * ((width/cx)+1);
             
             /*
        // Write a ppm version to demo
	fd = open("smpl.ppm", O_CREAT | O_WRONLY, 0666);
	char buf[1024];
	rc = sprintf(buf, "P6 %d %d 255\n", width, height);
	write(fd, buf, rc); // Write the PPM image header before data
	write(fd, bmp_buffer, bmp_size); // Write out all RGB pixel data
	close(fd);
        */

	free(bmp_buffer); // The decompressed image

	return 0;
}
