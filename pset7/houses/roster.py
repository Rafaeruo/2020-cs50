from sys import argv
from cs50 import SQL

def main():
    if len(argv) != 2:
        print("Review command-line arguments.")
        exit(1)

    db = SQL("sqlite:///students.db")

    rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house=? ORDER BY last, first", argv[1])

    if len(rows) == 0:
        exit(0)
    for row in rows:
        if row["middle"] != None:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
        else:
            print(f"{row['first']} {row['last']}, born {row['birth']}")


main()