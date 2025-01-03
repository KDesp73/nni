#ifndef COMMANDS_H
#define COMMANDS_H
#define ANSI_IMPLEMENTATION
#include <io/ansi.h>
#include <stddef.h>
#include <stdio.h>

typedef enum {
    COMMAND_NN,
    COMMAND_LOAD,
    COMMAND_SAVE,
    COMMAND_SET,
    COMMAND_FEED,
    COMMAND_HELP,
    COMMAND_QUIT,
    COMMAND_TRAIN,
    COMMAND_PRINT,
    COMMAND_CLEAR,
} Command;

Command StringToCommand(char* str);


void HandleCommand(char* input);

void CommandPrint(char** tokens, size_t count);
void CommandNN(char** tokens, size_t count);
void CommandLoad(char** tokens, size_t count);
void CommandSave(char** tokens, size_t count);
void CommandFeed(char** tokens, size_t count);
void CommandSet(char** tokens, size_t count);
static inline void CommandHelp()
{
    printf("%sCOMMANDS%s\n", ANSI_BOLD, ANSI_RESET);
    printf("  nn <LAYER-NEURONS>...\n");
    printf("  load <TYPE> <PATH>\n");
    printf("  save <PATH>\n");
    printf("  train\n");
    printf("  feed <INPUT-VALUES>...\n");
    printf("  set <KEY> <VALUE>\n");
    printf("  print <VALUE>\n");
    printf("  clear\n");
    printf("  help\n");
    printf("  quit\n");
}


#endif // COMMANDS_H
