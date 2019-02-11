#!/usr/bin/python3
import psycopg2
import code

def main():
    conn=psycopg2.connect("dbname=framedb")
    cur = conn.cursor()

    cur.execute("SELECT * from csv_lines ORDER BY line_id;")

    ou = open("output.csv",'wb')
    while 1:
        tmp = cur.fetchone()
        if tmp is None:
            break
        ou.write(tmp[2].tobytes()+b'\n')

    cur = conn.cursor()
    cur.execute("DELETE from csv_lines;")
    conn.close()

if __name__=="__main__":
    main()
