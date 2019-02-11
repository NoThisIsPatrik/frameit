#!/usr/bin/python3
import psycopg2
import code
from sys import argv

def main():
    conn=psycopg2.connect("dbname=framedb")
    cur = conn.cursor()

    cur.execute("SELECT * from csv_lines ORDER BY line_id;")
    if len(argv)<2:
        print("Usage: outputmaker.py <output file>")
        exit()

    ou = open(argv[1],'wb')
    while 1:
        tmp = cur.fetchone()
        if tmp is None:
            break
        ou.write(tmp[2].tobytes()+b'\n')

    cur = conn.cursor()
    cur.execute("DELETE from csv_lines;")
    conn.commit()
    conn.close()

if __name__=="__main__":
    main()
