#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

string validate_key(string key);
string cipher(string plain, string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("requires one argument (key)\n");
        return 1;
    }
    
    
    string key = argv[1];
    
    string error = validate_key(key);
    
    if (strcmp(error, "") != 0)//if the string error == "", the function outputs 0. if the error == "" it means the key is eligible
    {
            printf("%s", error);
            return 1;
    }
    
    string plaintext = get_string("plaintext: ");
    string ciphertext = cipher(plaintext, key);
    
    printf("ciphertext: %s\n", ciphertext);
    
    
    
    
    
    
}

string validate_key(string key)
{
    if (strlen(key) != 26)//check the key length
    {
        return "key must be 26 characters long\n";
    }
    
    for (int i = 0; i < 26; i++)//check if there is a repeating character or a non alphabetic character
    {
        if ((key[i] >= 65 && key[i] <= 90) || (key[i] >= 97 && key[i] <= 122))
        {
            for (int j = 0; j < 26; j++)
            {
                if (key[i] == key[j] && i != j){
                    return "key must not repeat characters\n";
                }
            }
        }
        else
        {
            return "key must only contain alphabetical characters\n";
        }
    }
    
    return "";
}

string cipher(string plain, string key)
{
    for (int i = 0; i < strlen(plain); i++)
    {
        if ((int) plain[i] >= 65 && plain[i] <= 90)//uppercase
        {
            plain[i] = toupper(key[(int) plain[i]-65]);
        }
        else if ((int) plain[i] >= (65+32) && plain[i] <= (90+32))//upercase
        {
            plain[i] = tolower(key[(int) plain[i]-(65+32)]);
        }
        
    }
    
    return plain;
}
