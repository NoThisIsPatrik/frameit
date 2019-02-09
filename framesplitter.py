#!/usr/bin/python3
import av
import av.datasets
import os
import psycopg2
import code
import io

def main():
    conn=psycopg2.connect("dbname=framedb")
    cur = conn.cursor()
    sql = """INSERT INTO frames( file_name, cell_x, cell_y, frame_time, frame_data)
                 VALUES(%s, %s, %s, %s, %s) RETURNING frame_id;"""

    fns = [("/c/me/potassium.MOV",64,64)]
    for (fn,sx,sy) in fns:
        print(f"Extracting kfs from {fn}")
        container = av.open(av.datasets.curated(fn))
        stream = container.streams.video[0] 
            # TODO Arbitrary useing first stream, check others?
        stream.codec_context.skip_frame = "NONKEY"

        n = 10 # TODO Do all frames instead of first n, here for short debug runs

        for frame in container.decode(video=0):
            bio = io.BytesIO()
            frame.to_image().save(bio,format="jpeg")
            ba = bio.getvalue()
            cur.execute(sql, (fn, str(sx), str(sy), "%.2f"%(frame.time), ba))
            # code.interact(local=dict(globals(),**locals()))
            fid = cur.fetchone()[0]
            print(f"Frame put in {fid}")

                # TODO remove frame limit
            n -= 1
            if not n:
                break
           
        cur.close()
        conn.commit()
if __name__=="__main__":
    main()
