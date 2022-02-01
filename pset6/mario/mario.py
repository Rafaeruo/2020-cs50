from cs50 import get_int

while True:
    h = get_int("Height: ")
    if h>=1 and h<=8:
        break

for i in range(h):
    print(" "*(h-i-1), end="")
    print("#"*(i+1), end="")
    print("  ", end="")
    print("#"*(i+1))