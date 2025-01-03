#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_tokens(char** tokens, size_t count)
{
    if (!tokens) return;

    for (size_t i = 0; i < count; i++) {
        if (tokens[i]) {
            free(tokens[i]);
        }
    }
    free(tokens);
}

void print_tokens(char** tokens, size_t count)
{
    for(size_t i = 0; i < count; i++){
        if(tokens[i] != NULL)
            printf("%s\n", tokens[i]);
    }
}


char** tokenize(char* input, size_t* token_count) {
    if (!input || !token_count) {
        return NULL; // Handle invalid input
    }

    size_t count = 0;
    size_t capacity = 10; // Initial capacity for tokens
    char** tokens = malloc(capacity * sizeof(char*));
    if (!tokens) {
        return NULL; // Memory allocation failed
    }

    char* token = strtok(input, " "); // Get the first token
    while (token) {
        // Resize if needed
        if (count >= capacity) {
            capacity *= 2;
            char** new_tokens = realloc(tokens, capacity * sizeof(char*));
            if (!new_tokens) {
                free(tokens); // Free previously allocated memory
                return NULL; // Memory allocation failed
            }
            tokens = new_tokens;
        }

        tokens[count++] = strdup(token); // Store a copy of the token
        token = strtok(NULL, " ");      // Get the next token
    }

    *token_count = count; // Set the token count

    // Resize to fit the exact number of tokens
    char** result = realloc(tokens, count * sizeof(char*));
    return result ? result : tokens; // Return the resized array or original
}
