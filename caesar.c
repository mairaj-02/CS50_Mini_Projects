#include <stdio.h>
#include <ctype.h>
// #include <cs50.h> // this library only works in CS50's codeSpace.
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// main function takes arguments: argc and argv
int main(int argc, char *argv[])
{
    // Check if the number of command-line arguments is not equal to 2
    if (argc != 2)
    {
        // Print an error message indicating the correct usage
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else // If there are exactly 2 command-line arguments
    {
        // Initialize a boolean to check if the key is valid
        bool is_valid_key = true;

        // Iterate over each character in the second command-line argument (the key)
        for (int i = 0; argv[1][i] != '\0'; i++)
        {
            // Check if the character is not a digit
            if (!isdigit(argv[1][i]))
            {
                // Set the flag to false and break the loop if a non-digit character is found
                is_valid_key = false;
                break;
            }
        }

        // If the key is not valid (contains non-digit characters)
        if (!is_valid_key)
        {
            // Print the same error message indicating the correct usage
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Convert the valid key from a string to an integer using atoi
    int key = atoi(argv[1]);

    // Prompt the user to input the plaintext
    char* plaintext = get_string("Plaintext:  ");

    // Adjust the key if it is greater than 26 using modulo operation
    int effective_key = key % 26;
    // here if the key < 26, then according to the modulo property the effective_key == key

    // Print the ciphertext label
    printf("ciphertext: ");

    // Iterate over each character in the plaintext
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        if (isupper(plaintext[i]))
        {
            // Apply the Caesar cipher transformation for uppercase letters
            char cipher_char = ((plaintext[i] - 'A' + effective_key) % 26) + 'A';
            printf("%c", cipher_char);
        }
        // Check if the character is a lowercase letter
        else if (islower(plaintext[i]))
        {
            // Apply the Caesar cipher transformation for lowercase letters
            char cipher_char = ((plaintext[i] - 'a' + effective_key) % 26) + 'a';
            printf("%c", cipher_char);
        }
        else
        {
            // If the character is not a letter, print it as it is
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");

    return 0;
}