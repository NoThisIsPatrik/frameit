#!/usr/bin/python3
import psycopg2

def main():
    conn=psycopg2.connect("dbname=framedb")

    cur = conn.cursor()
    cur.execute("DELETE FROM csv_lines;")
    cur.execute("DELETE FROM frames;")
    cur.close()
    conn.commit()
    conn.close()

if __name__=="__main__":
    main()
