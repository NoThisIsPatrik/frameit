#include <iostream>
#include <pqxx/pqxx> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mkmedians.h"

using namespace std;
using namespace pqxx;

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
