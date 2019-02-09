# frameit
WIP Video Frame Proc
This is a very rough sketch of routine for extracting and processing frames from a video file.

It now uses postgres, expecting a postgresql db named framedb to exist.
It does not process frames in parallel
It is written in python3, not C/C++

There are there parts:

* initpostgres.py - Creates two tables, frames and csv_lines, to store frames and output lines in
* framesplitter.py - Splits a video file into keyframes, storing it in postgres under table "frames"
* frameprocessor.py - Process each keyframe, splitting into cells, calculating their median grayscale value, write the result to csv_lines, and deletes the frame from table frames
* outputmaker.py - Reads csv_lines and writes it to a csv file

Next up will be to redo frameprocessor.py in C++, and make it run in parallel.
