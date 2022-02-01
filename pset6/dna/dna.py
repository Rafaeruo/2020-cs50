from sys import argv
import csv

def main():
    if len(argv) != 3:
        print("Command-li arguments passed are incorrect.")
        exit(1)

    #open csv database file and read it into memory
    with open(argv[1], "r") as databaseF:
        reader = csv.DictReader(databaseF)
        database = []
        for row in reader:
            database.append(row)

    #open dna sequence file and read it into memory
    with open(argv[2], "r") as sequenceF:
        for i in sequenceF:
            sequence = i
    lenseq = len(sequence)


    keys = list(database[0].keys())#list of the keys of the database dictionary

    #create dictionary for storing number of repetitions for each stf key
    stfs = {}
    for i in range(1, len(keys)):
        stfs[keys[i]] = [0]*lenseq#for each stf key, create a len(sequence) long list of zeroes

    for stf in stfs:
        l = len(stf)#len of current stf
        for i, value in enumerate(sequence):
            if sequence[i:i+l] == stf:#if first substring after i is equal to the current stf
                previous = 0
                if (i) > 0:#if there is something behind the current index
                    previous = stfs[stf][i-1]#see how many times (if any) the stf has repeated until the last index
                stfs[stf][i+l-1] += previous+1#add the amounts of times it repeated to the end of the current substring

    #looks for the winner
    for row in database:
        counter = 0
        for stf in stfs:
            if max(stfs[stf]) == int(row[stf]):
                counter += 1
            else:
                break
        if counter == len(stfs):
            print(row["name"])
            exit()
    print("No Match")


main()