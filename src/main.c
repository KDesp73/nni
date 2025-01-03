#include <stddef.h>
#include <stdio.h>
#include <nn/activation.h>
#include <nn/config.h>
#include <nn/data.h>
#include <nn/methods.h>
#include <nn/misc.h>
#include <nn/network.h>
#include <nn/lists.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand((unsigned int)time(NULL));
    CONFIG_INIT();
    ConfigLoad("nn.conf");
    ConfigPrint();

    TrainingData data;
    TrainingDataLoad(&data, "data/or.dat");
    TrainingDataPrint(&data);

    Network n;
    NetworkInit(&n, SizesInit(data.inputCount, 3, data.outputCount, SIZESEND));

    TrainNetwork(&n, &data, config.epochs);
    NetworkdSave(&n, "nn-or.sav");

    // NetworkLoad(&n, "nn-xor.sav");
    NetworkPrint(&n);

    printf("\nTesting the trained network:\n");
    for (int i = 0; i < 4; i++) {
        double *input = data.inputs[i];
        for(size_t k = 0; k < data.inputCount; k++){
            n.layers[0].neurons[k].value = input[k];
        }

        Forward(&n);

        // Output result
        double output = n.layers[n.layerCount - 1].neurons[0].value;
        printf("Input: [%0.1f, %0.1f] -> Predicted: %0.4f, Target: %0.1f\n",
                input[0], input[1], output, data.outputs[i][0]);
    }

    NetworkFree(&n);
    TrainingDataFree(&data);

    return 0;
}
