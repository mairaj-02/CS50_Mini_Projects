#include <stdio.h>
// #include <cs50.h> // this library only works in CS50's codeSpace.
#include <string.h>

int main(void)
{
    long number;
    int digitCount;
    int digitsArray[16];  // Assuming the maximum possible digitCount is 16

        number = get_long("Enter your Credit Card number: ");

        // Extract and store each digit in the array
        long temp = number;
        digitCount = 0;
        int i = 0;
        while (temp != 0)
        {
            digitsArray[i] = temp % 10;
            temp /= 10;
            i++;
            digitCount++;
        }

               // Divide the array into two sub arrays
    int firstSize = (digitCount + 1) / 2;
    int firstArray[firstSize];
    int secondArray[digitCount - firstSize];

    int firstIndex = 0;
    int secondIndex = 0;

    for (int j = digitCount - 1; j >= 0; j -= 2)
    {
        firstArray[firstIndex] = digitsArray[j];
        firstIndex++;
    }

    for (int j = digitCount - 2; j >= 0; j -= 2)
    {
        secondArray[secondIndex] = digitsArray[j];
        secondIndex++;
    }
       //Multiply a specific array with 2 depending on the conditions
    if (digitCount % 2 == 0)
    {
        for (int j = 0; j < firstSize; j++)
        {
            firstArray[j] *= 2;
        }
    }
    else
    {
        for (int j = 0; j < digitCount - firstSize; j++)
        {
            secondArray[j] *= 2;
        }
    }
     //Add all the digits of the two arrays
    int totalSum = 0;

    for (int j = 0; j < firstSize; j++)
    {
        if (firstArray[j] < 10)
        {
            totalSum += firstArray[j];
        }
        else
        {
            totalSum += firstArray[j] / 10;  // Add the first digit
            totalSum += firstArray[j] % 10;  // Add the second digit
        }
    }

    for (int j = 0; j < digitCount - firstSize; j++)
    {
        if (secondArray[j] < 10)
        {
            totalSum += secondArray[j];
        }
        else
        {
            totalSum += secondArray[j] / 10;  // Add the first digit
            totalSum += secondArray[j] % 10;  // Add the second digit
        }
    }

    //Check the sum condition and card type
       if (totalSum % 10 == 0)
{
    if (digitCount == 15 && digitsArray[digitCount - 1] == 3 && (digitsArray[digitCount - 2] == 4 || digitsArray[digitCount - 2] == 7))
    {
        printf("AMEX\n");
    }
    else if (digitCount == 16 && digitsArray[digitCount - 1] == 5 && (digitsArray[digitCount - 2] >= 1 && digitsArray[digitCount - 2] <= 5))
    {
        printf("MASTERCARD\n");
    }
    else if ((digitCount == 13 || digitCount == 16) && digitsArray[digitCount - 1] == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
else
{
    printf("INVALID\n");
}

    return 0;
}