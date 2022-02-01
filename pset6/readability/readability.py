from cs50 import get_string

def main():
    text = get_string("Text: ")
    n = grade(text)

    if n < 1:
        print("Before Grade 1")
    elif n > 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(n)}")

def grade(text):
    letters = 0
    words = 1
    sentences = 0

    for i in text:
        c = ord(i)#get the ascii code for the char
        if (c >= 65 and c <= 90) or (c >= 97 and c <= 122):
            letters+=1
        elif i == " ":
            words+=1
        elif i in [".", "!", "?"]:
            sentences+=1

    avg_letters = (100/words) * letters
    avg_sentences = (100/words) * sentences

    #print(f"words: {words}")
    #print(f"letters: {letters}")
    #print(f"sentences: {sentences}")
    #print()
    #rint(f"avg letters: {avg_letters}")
    #print(f"avg sentences: {avg_sentences}")


    return 0.0588 * avg_letters - 0.296 * avg_sentences - 15.8

main()