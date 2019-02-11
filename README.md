# frameit
WIP Video Frame Proc
This is an increaingly less rough routine for processing keyframes from a video file.

It uses postgres, expecting a postgresql db named framedb to exist.
Some of it is written in python3, with key portions in C/C++

There are these parts:

* initpostgres.py - Creates two tables, "frames" and "csv_lines", to store frames and output lines in
* framesplitter.py - Splits a video file into keyframes, storing it in postgres under table "frames"
* (frameprocessor.py - Process each keyframe, splitting into cells, calculating their median grayscale value, write the result to "csv_lines", and deletes the frame from table "frames") This is now superceeded by cfp.cpp (C++ Frame Processor) which does the same thing, only in C++, and with slightly better read/write handling.
* cfp.cpp - Reads keyframes from PostgreSQL table "frames", calculates medians and writes them back into "csv_lines".
* outputmaker.py - Reads csv_lines and writes it to a csv file.
* cleandb.py - sweeps both tables clean. Use with care, but frequently needed during testing.

(procj.cpp - Uses libjpeg to decompress a jpeg file already in memory (as though it had been pulled from postgres) and calculates the median of specfied size cells. Prints csv with time as 0.00 as placeholder) This is now folded into cfp.cpp. It would probably make sense to keep it in a sparate file, will look into it when making Makefile. For now, I'd rather have too many things laying around than too few)
(test.jpg - A picture of my cat, Potassium, used by procj.cpp goes with procj.cpp, unlikely to be needed now)
