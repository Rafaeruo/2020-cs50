#include <stdio.h>
#include <cs50.h>

// program on receiving input and outputing a concateneted string - basic stuff

int main(void)
{
    string human_name = get_string("Quick, tell me your name!\n");
    printf("hello, %s\n", human_name);
}

//for compiling:
// make hello.c
//or clang -o whatever_filename hello.c