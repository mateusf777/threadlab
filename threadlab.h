#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct Pipes Pipes;
typedef struct StepParams StepParams;
typedef void (*StepFunc)(char *buffer);

struct Pipes {
    FILE *reader;
    FILE *writer;
};

struct StepParams {
    FILE *reader;
    FILE *writer;
    StepFunc stepFunc;
};

Pipes pipeFiles();

void *step(void *input);
