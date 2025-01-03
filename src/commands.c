#include "commands.h"
#include "log.h"
#include "state.h"
#include "tokenizer.h"
#include "utils.h"
#include <errno.h>
#include <nn/activation.h>
#include <nn/config.h>
#include <nn/data.h>
#include <nn/lists.h>
#include <nn/methods.h>
#include <nn/network.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Command StringToCommand(char* str)
{
#define IF_IS_COMMAND(string, name, command) \
    if(!strcmp(string, name)) return command

    IF_IS_COMMAND(str, "nn", COMMAND_NN);
    else IF_IS_COMMAND(str, "load", COMMAND_LOAD);
    else IF_IS_COMMAND(str, "save", COMMAND_SAVE);
    else IF_IS_COMMAND(str, "train", COMMAND_TRAIN);
    else IF_IS_COMMAND(str, "set", COMMAND_SET);
    else IF_IS_COMMAND(str, "print", COMMAND_PRINT);
    else IF_IS_COMMAND(str, "help", COMMAND_HELP);
    else IF_IS_COMMAND(str, "?", COMMAND_HELP);
    else IF_IS_COMMAND(str, "quit", COMMAND_QUIT);
    else IF_IS_COMMAND(str, "exit", COMMAND_QUIT);
    else IF_IS_COMMAND(str, "q", COMMAND_QUIT);
    else IF_IS_COMMAND(str, "feed", COMMAND_FEED);
    else return -1;

#undef IF_IS_COMMAND
}

void HandleCommand(char* input)
{
    size_t count;
    char** tokens = tokenize(input, &count);

    Command command = StringToCommand(tokens[0]);
    switch (command) {
    case COMMAND_NN:
        CommandNN(tokens, count);
        return;
    case COMMAND_LOAD:
        CommandLoad(tokens, count);
        return;
    case COMMAND_SAVE:
        CommandSave(tokens, count);
        return;
    case COMMAND_TRAIN:
        TrainNetwork(&state.nn, &state.data, config.epochs);
        info("Minimum loss reached %lf", state.nn.totalLoss);
        return;
    case COMMAND_FEED:
        CommandFeed(tokens, count);
        return;
    case COMMAND_SET:
        CommandSet(tokens, count);
        return;
    case COMMAND_PRINT:
        CommandPrint(tokens, count);
        return;
    case COMMAND_HELP:
        CommandHelp();
        return;
    case COMMAND_QUIT:
        exit(0);
    default:
        erro("Invalid command \"%s\". Try `help`", tokens[0]);
    }

    free_tokens(tokens, count);
}

void CommandPrint(char** tokens, size_t count)
{
    const char* value = tokens[1];
    if(!strcmp(value, "nn")){
        NetworkPrint(&state.nn);
    } else if(!strcmp(value, "data")){
        TrainingDataPrint(&state.data);
    } else if(!strcmp(value, "config")){
        ConfigPrint();
    } else if(!strcmp(value, "state")){
        printf("debug=%s\n", (state.debug) ? "true" : "false");
    }
}

void CommandSet(char** tokens, size_t count)
{
    if(count != 3) {
        erro("Command format should be `set <KEY> <VALUE>`");
        return;
    }

    const char* key = tokens[1];
    const char* value = tokens[2];
    
    ConfigSet(&config, key, value);
}

void CommandFeed(char** tokens, size_t count) {
    if(!state.nnLoaded){
        erro("Create a NN first");
        return;
    }

    size_t inputCount = state.nn.layers[0].neuronCount;

    if (count - 1 < inputCount) {
        erro("Provide %zu inputs", inputCount);
        return;
    }

    for (size_t i = 0; i < inputCount; i++) {
        char* num = tokens[i + 1];
        if (!isFloat(num)) {
            erro("\"%s\" is not a valid float.", num);
            return;
        }
        state.nn.layers[0].neurons[i].value = atof(num);
    }

    Forward(&state.nn);

    size_t outputCount = state.nn.layers[state.nn.layerCount - 1].neuronCount;
    for (size_t i = 0; i < outputCount; i++) {
        double output = state.nn.layers[state.nn.layerCount - 1].neurons[i].value;
        printf("%lf ", output);
    }
    printf("\n");
}

void CommandSave(char** tokens, size_t count)
{
    if(!state.nnLoaded){
        erro("Create a NN first");
        return;
    }

    if(count != 2) {
        erro("Command format should be `save <PATH>`");
        return;
    }

    char* path = tokens[1];
    NetworkdSave(&state.nn, path);
    info("Neural Network saved at %s", path);
}

void CommandLoad(char** tokens, size_t count) 
{    
    if (count != 3) {
        erro("Command format should be `load <TYPE> <PATH>`");
        return;
    }

    const char* filePath = tokens[2];

    if (!fileExists(filePath)) {
        erro("File '%s' does not exist or cannot be accessed: %s", filePath, strerror(errno));
        return;
    }

    if (!strcmp(tokens[1], "data")) {
        if(!state.nnLoaded){
            erro("Create a NN first");
            return;
        }

        TrainingDataLoad(&state.data, filePath);

        bool inputDiff = state.data.inputCount != state.nn.layers[0].neuronCount;
        bool outputDiff = state.data.outputCount != state.nn.layers[state.nn.layerCount - 1].neuronCount;

        if (inputDiff || outputDiff) {
            NetworkFree(&state.nn);

            Sizes sizes = SizesAlloc(state.nn.layerCount);

            if (inputDiff) {
                sizes.items[0] = state.data.inputCount;
            } else {
                sizes.items[0] = state.nn.layers[0].neuronCount;
            }

            if (outputDiff) {
                sizes.items[state.nn.layerCount - 1] = state.data.outputCount;
            } else {
                sizes.items[state.nn.layerCount - 1] = state.nn.layers[state.nn.layerCount - 1].neuronCount;
            }

            // Preserve intermediate layers
            for (size_t i = 1; i < state.nn.layerCount - 1; i++) {
                sizes.items[i] = state.nn.layers[i].neuronCount;
            }

            NetworkInit(&state.nn, sizes);

            if (inputDiff) {
                warn("Data loaded required %zu input neurons. NN modified to match requirements.", state.data.inputCount);
            }
            if (outputDiff) {
                warn("Data loaded required %zu output neurons. NN modified to match requirements.", state.data.outputCount);
            }
        }

        info("Data loaded from %s.", filePath);
        return;
    }

    if (!strcmp(tokens[1], "nn")) {
        NetworkFree(&state.nn);
        NetworkLoad(&state.nn, filePath);
        state.nnLoaded = true;
        info("Neural network loaded from %s.", filePath);
        return;
    }

    erro("Unknown type '%s'. Valid types are 'data' and 'nn'.", tokens[1]);
}

void CommandNN(char** tokens, size_t count)
{
    if (count < 4) {
        erro("Provide at least 3 layers (e.g., 2 2 1)");
        return;
    }

    Sizes sizes = SizesAlloc(count - 1);

    for (size_t i = 1; i < count; i++) {
        char* num = tokens[i];
        if (!isNumber(num)) {
            erro("\"%s\" is not a valid positive integer.", num);
            SizesFree(&sizes);  // Free allocated memory before returning
            return;
        }

        int neuronCount = atoi(num);
        if (neuronCount <= 0) {
            erro("\"%s\" must be a positive integer.", num);
            SizesFree(&sizes);
            return;
        }

        sizes.items[i - 1] = neuronCount;
    }

    NetworkFree(&state.nn);
    NetworkInit(&state.nn, sizes);
    state.nnLoaded = true;
    info("Neural Network created.");
}

