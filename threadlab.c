#include "threadlab.h"

Pipes pipeFiles() {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe ");
        exit(1);
    }

    FILE *pipeReader = fdopen(fd[0], "r");
    FILE *pipeWriter = fdopen(fd[1], "w");

    Pipes pipes = {pipeReader, pipeWriter};
    return pipes;
}

void *step(void *input) {
    StepParams *params = (StepParams *) input;
    FILE *reader = params->reader;
    FILE *writer = params->writer;
    StepFunc stepFunc = params->stepFunc;

    size_t n1, n2;

    char buffer[BUFFER_SIZE];
    while (1) {
        n1 = fread(buffer, 1, BUFFER_SIZE, reader);
        if (n1 <= 0) break;

        char r[n1 + 1], rl[n1 + 1];
        memcpy(r, buffer, n1);

        if (stepFunc != NULL) {
            stepFunc(r);
        }

        n2 = fwrite(r, 1, n1, writer);
        if (n2 < 0) break;
        fflush(writer);
    }

    return NULL;
}
