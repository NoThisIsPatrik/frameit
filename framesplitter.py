#!/usr/bin/python3
import av
import av.datasets
import os

def main():
    fns = ["/c/me/potassium.MOV"]
    for fn in fns:
        print(f"Extracting kfs from {fn}")
        container = av.open(av.datasets.curated(fn))
        stream = container.streams.video[0] # TODO Arbitrary useing first stream, check others?
        stream.codec_context.skip_frame = "NONKEY"

        n = 10
        dirname = f'./tmp_{fn.replace("/","_")}'

        if not os.path.isdir(dirname):
            os.mkdir(dirname)

        for frame in container.decode(video=0):
            fn = f"{dirname}/f{frame.time:.2f}"
            fn = fn[:-3] + "_" + fn[-2:]
                # TODO Use different storage, such as db (postgres, nosql, something) 

            if os.path.isfile(f'{fn}.jpg') or os.path.isfile(f'{fn}.csv'):
                continue

            print(f"Saving {fn}")
            frame.to_image().save(f"{fn}.jpg")

if __name__=="__main__":
    main()
