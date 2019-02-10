#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]) {
   try {
      connection C("dbname = framedb");

      if (C.is_open()) {
         cout << "Starting" << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      nontransaction N(C);
      result R( N.exec( "SELECT * from frames" ) );

      for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        uint32_t frame_id,cell_x,cell_y;
        string filename;
        string img_dat;
        FILE *ut;

        frame_id = c[0].as<int>();
        filename = c[1].as<string>();
        cell_x = c[2].as<int>();
        cell_y = c[3].as<int>();
        pqxx:binarystring blob(c["frame_data"]);
        ut = fopen("test.jpg","w");
        fwrite(blob.data(),1,blob.size(),ut);
        fclose(ut);
        
        cout << frame_id << " " << filename << " (" << cell_x << "," << cell_y << ") " << blob.size() << endl;
        break;

      }

      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }
}
