#!/usr/bin/python3
import av
import av.datasets
import os
import psycopg2
import io
from sys import argv

def main():
    conn=psycopg2.connect("dbname=framedb")
    cur = conn.cursor()
    if len(argv)<4:
        print("USAGE:  framesplitter.py <input video> <cell width> <cell height>")
        exit()

    fn = argv[1]
    if not os.path.isfile(fn):
        print("Input file doesn't exist or isn't a file.")
        exit()

    sx = int(argv[2])
    sy = int(argv[3])

    sql = """INSERT INTO frames( file_name, cell_x, cell_y, frame_time, frame_data)
                 VALUES(%s, %s, %s, %s, %s) RETURNING frame_id;"""

    print(f"Extracting keyframes from {fn}")
    container = av.open(av.datasets.curated(fn))
    stream = container.streams.video[0] 
        # TODO Arbitrarily using first stream, check others perhaps?

    stream.codec_context.skip_frame = "NONKEY"

    for frame in container.decode(video=0):
        bio = io.BytesIO()
        frame.to_image().save(bio,format="jpeg")
        ba = bio.getvalue()
        cur.execute(sql, (fn, str(sx), str(sy), "%.2f"%(frame.time), ba))
        # code.interact(local=dict(globals(),**locals()))
        fid = cur.fetchone()[0]
        print(f"Frame put in {fid}")
       
    cur.close()
    conn.commit()

if __name__=="__main__":
    main()
