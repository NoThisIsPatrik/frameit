# frameit
WIP Video Frame Proc
This is a very rough sketch of routine for extracting and processing frames from a video file.

It uses a temp dir and files instead of a database or similar.
It does not process frames in paralell (though they run in such a way that they could, and run linearly)
It is written in python3, not C/C++

As such, it isn't partcularly good, but in the interest of keeping this similar to how I would treat a normal assigned task (sans per haps talking through my overview more before even going here), I approach it as I normally would. That would in all likelyhood be to write a semi-functional sketch in python/bash/whatnot and getting it more or less running, then working from there.

There are there parts:

* framesplitter.py - Splits a video file into keyframes, storing it in a temp dir.
* frameprocessor.py - Process each keyframe, splitting into cells, calculating their median grayscale value, and writing it to a .csv file snipplet by the same name, and removes the keyframe file
* outputmaker.py - joins the output from frameprocessor into a single .csv file

This is all quite hackish, but as mentioned, this would be my step one.
