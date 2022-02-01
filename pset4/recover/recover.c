#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)//verifying if the correct amount of arguments was passed (1)
    {
        printf("One, and only one, argument should be passed.\n");
        return 1;
    }
    
    FILE *f = fopen(argv[1], "r");//open the input file
    
    if (f == NULL)
    {
        //errno is a variable created by its library and has its value set by functions upon errors.
        printf("Could not open the file specified in the argument. Error code: %i.\n", errno);
        return 1;
    }
    
    BYTE *buffer = malloc(512);//where the 512 byte blocks will be stored
    char *filename = malloc(7);// (i.e. 000.jpg)
    FILE *f2 = NULL;//where each jpeg will be intialized
    int counter = -1;//number of jpegs created to determine the name of each of them and if it's the first one being created
    while (fread(buffer, 512, 1, f) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)//checking if it's the start of a new jpeg
        {
            counter++;
            if (counter != 0)//if not the first jpeg
            {
                fclose(f2);
            }
            sprintf(filename, "%03i.jpg", counter);//sets filename to a string: ###.jpg
            f2 = fopen(filename, "w");//create the jpg we are writing to
            
            fwrite(buffer, 512, 1, f2);//write the first block of 512 bytes to the jpeg file
        }
        else//if it's not the start of a new jpeg
        {
            if (counter > -1)//if there is a jpeg being created
            {
                fwrite(buffer, 512, 1, f2);//write the new 512 byte block to the jpeg file
            }
        }
    }
    
    
    free(filename);//free the 7 bytes used to store the filenames
    free(buffer);//free the 512 bytes used to buffer the current 512 byte "blocks" being read
    return fclose(f);//returning this because the fclose funcion, when correctly executed, returns 0;//close the input file
}
