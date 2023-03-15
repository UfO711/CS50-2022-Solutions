# TODO
from cs50 import get_string

def main():
    number = get_string("Number: ")
    if len(number) in [15, 16, 13]:
        if len(number) == 15 and number[:2] in ["34", "37"]:
            if luhn_algo(number) == 0:
                print("AMEX")
        elif len(number) == 16 and number[:2] in ["51", "52", "53", "54", "55"]:
            if luhn_algo(number) == 0:
                print("MASTERCARD")
        elif (len(number) == 13 or len(number) == 16) and number[0] == "4":
            if luhn_algo(number) == 0:
                print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")

def luhn_algo(n):
    sum = 0
    # If number have even number of digit, digits that need to multiply 2 have index start with 1
    if len(n) % 2 == 0:
        for i in range(len(n)):
            # Digits with index of odd number do not have to multiply by 2
            if i % 2 != 0:
                sum = sum + int(n[i])
            else:
                if int(n[i]) > 4:
                    sum = sum + 1 + (int(n[i]) - 5) * 2
                else:
                    sum = sum + int(n[i]) * 2
        return sum % 10
    # If number have odd number of digit, digits that need to multiply 2 have index start with 0
    else:
        for i in range(len(n)):
            # Digits with index of even number do not have to multiply by 2
            if i % 2 == 0:
                sum = sum + int(n[i])
            else:
                if int(n[i]) > 4:
                    sum = sum + 1 + (int(n[i]) - 5) * 2
                else:
                    sum = sum + int(n[i]) * 2
        return sum % 10

if __name__ == "__main__":
    main()
