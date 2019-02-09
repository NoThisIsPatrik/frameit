#!/usr/bin/python3
from PIL import Image
import io
import psycopg2
import code

def set_cut_points(fn,zx,zy):
    global px,py,sx,sy

    sx,sy = zx,zy
    I = Image.open(fn)

def process_frame(fn):
    global px,py,sx,sy

    I = Image.open(fn)
    md = [int(sorted(sum(I.getpixel((cx+x,cy+y))) for x in range(0,sx) for y in range(0,sy))[int(sx*sy/2)]/3) for cx in px for cy in py]
    ts=fn.rsplit('/f',1)[1].split(".",1)[0].replace('_','.')
    return (f"{ts},{','.join(str(a) for a in md)}")

def main():
    conn=psycopg2.connect("dbname=framedb")
    sql = """INSERT INTO csv_lines( line_id, file_name, csv_line)
                 VALUES(%s, %s, %s);"""

    cur = conn.cursor()
    cur.execute("SELECT * from frames;")
    cur2 = conn.cursor()
    cur3 = conn.cursor()

    while 1:
        tmp = cur.fetchone()
        if tmp is None:
            break
        frame_id,frame_filename,cell_x,cell_y,frame_time,img_data = tmp
        print(f"Processing {frame_id}")
        I = Image.open(io.BytesIO(img_data))
        x,y = I.size
        px = [i*cell_x for i in range(int(x/cell_x))]
        py = [i*cell_y for i in range(int(y/cell_y))]
        md = [int(sorted(sum(I.getpixel((cx+x,cy+y))) for x in range(0,cell_x) for y in range(0,cell_y))[int(cell_x*cell_y/2)]/3) for cx in px for cy in py]
        ts = str(frame_time)
        csv_line = (f"{ts},{','.join(str(a) for a in md)}")
        cur2.execute(sql,(frame_id,frame_filename,csv_line))
        print(f"{frame_id} {frame_filename} {csv_line:.20s}")
        # cur3.execute("DELETE FROM frames WHERE frame_id=%s;",(frame_id,))

        # code.interact(local=dict(globals(),**locals()))
    cur3.close()
    cur2.close()
    cur.close()
    conn.commit()

if __name__=="__main__":
    main()
