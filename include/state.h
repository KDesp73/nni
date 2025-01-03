#ifndef STATE_H
#define STATE_H

#include <nn/data.h>
#include <nn/network.h>
#include <stdbool.h>
typedef struct {
    Network nn;
    TrainingData data;
    bool nnLoaded;
    bool debug;
} State;

extern State state;

#endif // STATE_H
