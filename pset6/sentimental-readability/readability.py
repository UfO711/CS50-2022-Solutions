# TODO
from cs50 import get_string

text = get_string("Text: ")
# Initialize variables
sum_letters = 0
sum_sentences = 0
sum_words = 1

# Loop over every characters in the string
# Calculate number of letters, sentences, words
for c in text:
    if c.isalpha():
        sum_letters += 1
    if c == "!" or c == "?" or c == ".":
        sum_sentences += 1
    if c == " ":
        sum_words += 1

# Calculate L and S
l = (sum_letters / sum_words) * 100
s = (sum_sentences / sum_words) * 100

# Coleman-Liau index in short
cl_index = 0.0588 * l - 0.296 * s - 15.8

# Print answer
if cl_index < 1:
    print("Before Grade 1")
elif cl_index > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(cl_index)}")


