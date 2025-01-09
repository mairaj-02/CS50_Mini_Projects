#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define the maximum length of a word
#define LENGTH 45

// Define a node structure for the linked list implementation of the hash table
typedef struct node {
    char word[LENGTH + 1]; // Store the word (plus one for null terminator)
    struct node *next;      // Pointer to the next node in the linked list
} node;

// Global variables
unsigned int word_counter = 0; // Count of words loaded into the dictionary
const unsigned int table_size = 26 * 26 * 26; // Size of the hash table (for three-letter combinations)
const unsigned int max_hash_value = table_size - 1; // Maximum hash value for indexing
bool checking_load = false; // Flag to indicate if the dictionary has been loaded
node* table[table_size] = {NULL}; // Hash table initialized to NULL

// Function prototypes
unsigned int hash(const char *word);
unsigned int size(void);
bool load(const char* word);
bool unload(void);
bool check(const char *word);

int main(int argc, char *argv[]) {
    // Ensure proper usage with command-line argument for dictionary file
    if (argc != 2) {
        printf("Usage: ./speller dictionary\n");
        return 1;
    }

    // Measure time taken to load the dictionary
    clock_t load_start = clock();
    bool result = load(argv[1]);
    if (!result) {
        printf("Could not load dictionary.\n");
        return 2;
    }
    clock_t load_end = clock();

    // Calculate and display loading time
    double load_seconds = (double)(load_end - load_start) / CLOCKS_PER_SEC;
    printf("Time to load dictionary: %f seconds\n", load_seconds);
    
    // Display the number of words loaded into the dictionary
    printf("Number of words loaded: %i\n", size());

    // Loop to allow user input for spell checking
    bool input_condition = true;
    while(input_condition) {
        printf("Enter a word to check the spelling: ");
        char word[LENGTH + 1];
        scanf("%45s", word); // Read user input, ensuring no overflow

        // Measure time taken to check the spelling of the word
        clock_t check_start = clock();
        bool check_result = check(word);
        if (check_result) {
            printf("The word is in the dictionary.\n");
        } else {
            printf("The word is not in the dictionary.\n");
        }
        clock_t check_end = clock();

        // Calculate and display checking time
        double check_seconds = (double)(check_end - check_start) / CLOCKS_PER_SEC;
        printf("Time to check word: %f seconds\n", check_seconds);
        
        // Ask user if they want to check another word
        char input[4];
        printf("Would you like to check another word? (y/n)\n");
        scanf("%3s", input);
        if (input[0] == 'n' || input[0] == 'N') {
            input_condition = false; // Exit loop if user inputs 'n' or 'N'
        }
    }

    unload(); // Free memory used by the hash table before exiting
    return 0;
}

// Hash function to compute an index for a given word based on its first three letters
unsigned int hash(const char *word) {
    // Handle words with three or more characters
    if (strlen(word) >= 3) {
        if (isalpha(word[2]) && isalpha(word[1]) && isalpha(word[0])) {
            int first_word = toupper(word[0]) - 'A'; // Convert first character to index
            int second_word = toupper(word[1]) - 'A'; // Convert second character to index
            int third_word = toupper(word[2]) - 'A'; // Convert third character to index
            
            unsigned int hash_value = ((676 * first_word + 26 * second_word + 1 * third_word)) % max_hash_value;
            return hash_value;            
        }
        else if (!isalpha(word[0])) {
            return ((676 * 0) + 0 + 0) % max_hash_value; // Handle non-alphabetic first character
        }
        else if (!isalpha(word[1])) {
            int first_word = toupper(word[0]) - 'A';
            return ((676 * first_word) + 0 + 0) % max_hash_value; // Handle non-alphabetic second character
        }
        else if (!isalpha(word[2])) {
            int first_word = toupper(word[0]) - 'A';
            int second_word = toupper(word[1]) - 'A';
            return ((676 * first_word + 26 * second_word)) % max_hash_value; // Handle non-alphabetic third character
        }
    }
    
    // Handle words with two characters
    else if (strlen(word) == 2) {
        if (isalpha(word[1])) {
            int first_word = toupper(word[0]) - 'A';
            int second_word = toupper(word[1]) - 'A';
            return ((676 * first_word + 26 * second_word)) % max_hash_value;
        } else {
            int first_word = toupper(word[0]) - 'A';
            return ((676 * first_word)) % max_hash_value; // Handle non-alphabetic second character
        }
    }
    
    // Handle single-character words
    else if (strlen(word) == 1) {
        int first_word = toupper(word[0]) - 'A';
        return ((676 * first_word)) % max_hash_value;
    }

    return 0; // Return zero for empty or invalid strings
}

// Returns the number of words currently loaded in the dictionary, or zero if not loaded
unsigned int size(void) {
    return checking_load ? word_counter : 0; 
}

// Loads a dictionary into memory, returning true if successful, false otherwise
bool load(const char* dictionary) {
    FILE* temp = fopen(dictionary, "r"); // Open the dictionary file for reading
    if (temp == NULL) {
        return false; // Return false if file could not be opened
    }

    char temp_word[LENGTH + 1]; // Temporary storage for each word read from file

    while(fscanf(temp, "%45s", temp_word) != EOF) { 
        node* word_node = malloc(sizeof(node)); // Allocate memory for a new node
        
        if (word_node == NULL) { 
            fclose(temp); 
            return false; // Return false if memory allocation fails
        }    

        unsigned int hash_value = hash(temp_word); // Compute hash value for the current word
        
        strcpy(word_node->word, temp_word); // Copy the word into the node
        
        node* cursor = table[hash_value]; 
        word_node->next = cursor;         // Insert new node at the beginning of linked list
        
        table[hash_value] = word_node;     // Update hash table entry with new node
        
        word_counter++;                    // Increment global counter of loaded words 
    }

    checking_load = true;                // Set flag indicating that loading has completed successfully
    
    fclose(temp);                        // Close the file after loading all words
    
    return true;                         // Return true indicating successful loading of dictionary
}

// Checks whether a given word is in the dictionary, returning true if found, false otherwise
bool check(const char *word) {
    unsigned int hash_value = hash(word);   // Compute hash value for the input word
    
    node* cursor = table[hash_value];       // Start searching from the head of linked list at computed index
    
    while (cursor != NULL) {                 // Traverse linked list until end is reached or match found 
        if (strcasecmp(word, cursor->word) == 0) { 
            return true;                     // Return true if a match is found (case insensitive)
        }
        
        cursor = cursor->next;               // Move to next node in linked list 
    }

    return false;                            // Return false if no match was found after traversing list 
}

// Unloads all nodes from memory and frees allocated resources, returning true on success.
bool unload(void) {
    for (int i = 0; i < table_size; i++) {   // Iterate over each bucket in hash table 
        node* cursor = table[i];             
        
        while (cursor != NULL) {              // Traverse linked list in current bucket 
            node* temp = cursor;             
            
            cursor = cursor->next;            // Move to next node before freeing current one
            
            free(temp);                       // Free memory allocated for current node 
        }
    }
    
    return true;                             // Return true indicating successful unload operation 
}
