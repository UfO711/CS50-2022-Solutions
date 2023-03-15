# TODO
from cs50 import get_int

# Prompt the user for input
while True:
    height = get_int("Height: ")
    # Once height is no greater than 8, stop the loop
    if height <= 8 and height > 0:
        break;

block = "#"
space = " "
# Using string catenation in python
for i in range(height):
    print(space * (height - 1 - i) + block * (i + 1) + space * 2 + block * (i + 1))

