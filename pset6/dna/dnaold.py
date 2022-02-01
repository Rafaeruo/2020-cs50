from sys import argv
import csv

def main():
    if len(argv) != 3:
        print("Command-li arguments passed are incorrect.")
        exit(1)

    databaseF = open(argv[1], "r")
    reader = csv.reader(databaseF)
    database = []
    for row in reader:
        database.append(row)
    databaseF.close()

    sequenceF = open(argv[2], "r")
    for i in sequenceF:
        sequence = i
    sequenceF.close()

    lenseq = len(sequence)
    stfs = {"AGATC": [0]*lenseq,"TTTTTTCT": [0]*lenseq,"AATG": [0]*lenseq,"TCTAG": [0]*lenseq,"GATA": [0]*lenseq,"TATC": [0]*lenseq,"GAAA": [0]*lenseq,"TCTG": [0]*lenseq}


    for stf in stfs:
        l = len(stf)
        for i in range(lenseq):
            if (i+l) > (lenseq-1):
                #print("parei 1")
                break
            subs = sequence[i:i+l]
            #print(f"initial substring: {i}:{i+l}")
            counter = 0

            if subs == stf and stfs[stf][i+l-1] == 0:
                #print(f"starting index: {i}")
                counter = 1
                while subs == stf:
                    if (l+i+(l*(counter-1))) > lenseq and stfs[stf][i+(l*counter)-1] != 0:
                        #print("out of range")
                        break
                    #print(f"counter: {counter}")
                    #print(f"storing counter in: {i+(l*counter)-1}")
                    stfs[stf][i+(l*counter)-1] = counter
                    counter+=1
                    #print(f"next substring: {i+(l*(counter-1))}:{l+i+(l*(counter-1))}")
                    subs = sequence[i+(l*(counter-1)) : l+i+(l*(counter-1))]


    for i in range(1, len(database)):
        counter = 0
        for j in range(1, len(database[i])):
            if int(database[i][j]) == max(stfs[database[0][j]]):
                counter += 1
            else:
                break
        if counter == (len(database[0])-1):
            print(database[i][0])
            break
        elif i == (len(database)-1):
            print("No match")

main()