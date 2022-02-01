from sys import argv
import csv
from cs50 import SQL

def main():
    if len(argv) != 2:
        print("Review command-line arguments.")
        exit(1)

    with open(argv[1], "r") as f:
        reader = csv.DictReader(f)
        table = []
        for row in reader:
            table.append(row)

    db = SQL("sqlite:///students.db")

    for row in table:
        name = row["name"].split(" ")
        house = row["house"]
        birth = row["birth"]
        if len(name) == 2:
            first = name[0]
            middle = None
            last = name[1]
        else:
            first = name[0]
            middle = name[1]
            last = name[2]

        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", first, middle, last, house, birth)


main()