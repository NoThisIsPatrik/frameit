#!/usr/bin/python3
import psycopg2

def main():
    conn=psycopg2.connect("dbname=framedb")
    mktable(conn)


def mktable(conn):
    cur = conn.cursor()
    cmds = (
    """
        CREATE TABLE frames (
                frame_id SERIAL PRIMARY KEY,
                file_name VARCHAR(255),
                cell_x INTEGER,
                cell_y INTEGER,
                frame_time NUMERIC (5,2),
                frame_data BYTEA NOT NULL
        )
    """,
    """
        CREATE TABLE csv_lines (
                line_id INTEGER PRIMARY KEY,
                file_name VARCHAR(255),
                csv_line BYTEA NOT NULL
        )
    """,
    )
    for c in cmds:
        try:
            print(c)
            cur.execute(c)
        except psycopg2.ProgrammingError as e:
            print(e)
            cur.close()
            conn.commit()
            cur = conn.cursor()
    cur.close()
    conn.commit()

if __name__=="__main__":
    main()

