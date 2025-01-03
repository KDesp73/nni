#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

char** tokenize(char* input, size_t *count);
void free_tokens(char** tokens, size_t count);
void print_tokens(char** tokens, size_t count);


#endif // TOKENIZER_H
