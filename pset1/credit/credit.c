#include <stdio.h>
#include <cs50.h>

string checksum(long n);

int main(void)
{
    long n = get_long("Number: ");
    //printf("%ld\n", n);

    printf("%s", checksum(n));




}

string checksum(long n)
{   
    
    long m = n;
    int digit_count = 0;
    
    while (n!=0){
        digit_count++;
        n /= 10;
    }
    
    
    n = m;
    n /= 10;
    int company_digits;

    int n2;
    int sum_n2;


    int sum = 0;
    while (n != 0 || m != 0)
    {
        if (n>9 && n<100){
            company_digits = n;
        }
        else if (m>9 && m<100){
            company_digits = m;
        }

        //printf("n:%li m:%li\n", n%10, m%10);
        n2 = (n % 10) * 2;

        sum_n2 = 0;
        while (n2 != 0)
        {
            sum_n2 += n2 % 10;
            n2 /= 10;
        }
        sum += m % 10;
        sum += sum_n2;

        n /= 100;
        m /= 100;
    }

    //printf("%i\n", sum);
    //printf("%i", company_digits);
    
    if (sum % 10 == 0){
        if ((company_digits == 34 || company_digits == 37) && digit_count == 15){
            return "AMEX\n";
        }
        else if ((company_digits > 50 && company_digits < 56) && digit_count == 16){
            return "MASTERCARD\n";
        }
        else if ((company_digits / 10 == 4 && digit_count > 12) && digit_count < 17){
            return "VISA\n";
        }
        else{
            //printf("aqui");
            return "INVALID\n";
        }
    }
    else{
        return "INVALID\n";
    }

}