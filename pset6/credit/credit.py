from cs50 import get_string

n = get_string("Number: ")

sum = 0
flag = False
c = 0
for i in reversed(n):
    c += 1
    if flag:
        var = str(2*int(i))
        if len(var) > 1:
            sum+=int(var[0]) + int(var[1])
        else:
            sum+=2*int(i)
        flag = False
    else:
        sum+=int(i)
        flag = True

if sum % 10 != 0:
    print("INVALID")
    exit()

company_two_digit = int(n[0]+n[1])
company_one_digit = int(n[0])

if company_two_digit in [34, 37] and c == 15:
    print("AMEX")
elif company_two_digit in [51, 52, 53, 54, 55] and c == 16:
    print("MASTERCARD")
elif company_one_digit == 4 and c in [13, 16]:
    print("VISA")
else:
    print("INVALID")