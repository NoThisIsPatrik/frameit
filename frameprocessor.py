#!/usr/bin/python3
from PIL import Image
import os
import glob 

def set_cut_points(fn,zx,zy):
    global px,py,sx,sy

    sx,sy = zx,zy
    I = Image.open(fn)
    x,y = I.size
    px = [i*sx for i in range(int(x/sx))]
    py = [i*sy for i in range(int(y/sy))]

def process_frame(fn):
    global px,py,sx,sy

    I = Image.open(fn)
    md = [int(sorted(sum(I.getpixel((cx+x,cy+y))) for x in range(0,sx) for y in range(0,sy))[int(sx*sy/2)]/3) for cx in px for cy in py]
    ts=fn.rsplit('/f',1)[1].split(".",1)[0].replace('_','.')
    return (f"{ts},{','.join(str(a) for a in md)}")

def main():
    dns = [ ("./tmp__c_me_potassium.MOV",(64,64)) ]
    
    for (dn,(sx,sy)) in dns:
        fns = glob.glob(dn + "/*.jpg")
            # TODO In addition to using afforementioned non-file storage,
            # this should be paralell/threaded. Some semaphorage will be needed if storage doesn't deal w/ it
        first_frame = True
        for fn in fns: 
            print(f"Processing frame {fn}")
            if first_frame:
                set_cut_points(fn,sx,sy)
                first_frame = False

            csvl = process_frame(fn)
            open(f"{fn[:-3]}csv",'w').write(csvl+"\n")
            os.remove(f"{fn}")

if __name__=="__main__":
    main()
