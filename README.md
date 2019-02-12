# frameit
WIP Video Frame Proc
This is a set of routines for processing keyframes from a video file.

It uses postgres, expecting a postgresql db named framedb to exist.

Some of it is written in python3, with key portions in C++. The actual processing should probably be in paralell, but how to achive that would depend a little on where this should run. A lot more handling would of course also be required, but again, as an example project without an actual flow in or out, setting up more of a surrounding seems a bit presumptive.

There are these parts:

* initpostgres.py - Creates two tables, "frames" and "csv_lines", to store frames and output lines in

* framesplitter.py - Splits a video file into keyframes, storing it in postgres under table "frames"

* cFrameprocessor.cpp - Reads keyframes from PostgreSQL table "frames", calls mkmedians to calculate medians, and writes csv lines to table "csv_lines"

* outputmaker.py - Reads table "csv_lines" and writes it to a csv file. Clears table "csv_lines"

* cleandb.py - sweeps both tables clean. Use with care, but frequently needed during testing.

(frameprocessor.py - Proceyframe, splitting into cells, calculating their median grayscale value, write the result to "csv_lines", and deletes the frame from table "frames") This is now superceeded by "frameprocessor", the C++ component actually processing the frames. It is still functional

Prerequisites:

python3
libav ("python3 -mpip install av")
PostgreSQL ("apt-get install postgresql-contrib)
libjpeg, libpqxx ("apt-get install libjpeg-dev libpqxx-dev")

BUILD:

-- These files shouldn't be here, and I'm looking for a sane way to remove them while making sure they don't vanish from the commit record.

(procj.cpp - Uses libjpeg to decompress a jpeg file already in memory as though it had been pulled from postgres, and calculates the median of specfied size cells. Prints csv with time as 0.00 as placeholder) This is now folded into cfp.cpp. It would probably make sense to keep it in a sparate file, will look into it when making Makefile. For now, I'd rather have too many things laying around than too few.

(test.jpg - A picture of my cat, Potassium, used by procj.cpp goes with procj.cpp, unlikely to be needed now)

