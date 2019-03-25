# frameit
Video Frame Processor

This is a set of routines for processing keyframes from a video file.

It uses postgres, expecting a postgresql db named framedb to exist.

One can be created with:

sudo -u postgres createdb framedb

Some of it is written in python3, with key portions in C++. The actual processing should probably be in parallel, but how to achive that would depend a little on where it would run. 

It's split up into these modules:

* initpostgres.py - Creates two tables, "frames" and "csv_lines", to store frames and output lines in

* framesplitter.py - Splits a video file into keyframes, storing it in postgres under table "frames"

* cFrameprocessor.cpp - Reads keyframes from PostgreSQL table "frames", calls mkmedians to calculate medians, and writes csv lines to table "csv_lines"

* outputmaker.py - Reads table "csv_lines" and writes it to a csv file. Clears table "csv_lines"

* cleandb.py - sweeps both tables clean. Use with care, but frequently needed during testing.

(frameprocessor.py - Proceyframe, splitting into cells, calculating their median grayscale value, write the result to "csv_lines", and deletes the frame from table "frames") This is now superceeded by "frameprocessor", the C++ component actually processing the frames. It is still functional

## Prerequisites:

python3, libav w/ python bindings, psycopg2, PostgreSQL, libjpeg, libpqxx

If they're not installed, they can be using:
### python3
apt-get install python3
### libav + python3 bindings
apt-get install libavformat-dev libavdevice-dev

python3 -mpip install av
### PIL (usually installed, but is used by libav)
python3 -mpip install pillow
### libjpeg, libpqxx (standard jpeg and postgres C/C++ libraries)
apt-get install libjpeg-dev libpqxx-dev

## BUILD:
git clone https://github.com/NoThisIsPatrik/frameit.git

make


## EXAMPLE RUN:

./initpostgres.py

./framesplitter.py cube.avi 64 64

./frameprocessor

./outputmaker.py cube.csv
