#include <pthread.h>
#include <assert.h>
#include "threadlab.h"

void toLower(char *str) {
    assert(str != NULL);
    char c;
    for(size_t i = 0; i < strlen(str); i++) {
        c = str[i];
        if(c >= 'A' && c <= 'Z') {
            str[i] = c - ('A' - 'a');
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }
    char *path = argv[1];

    // Pipes to link steps 1 and 2
    Pipes step1Pipes = pipeFiles();
    FILE *file = fopen(path, "r");
    if (file == NULL) return 1;
    // Prepare first step parameters
    StepParams step1 = {.writer = step1Pipes.writer, .reader = file};
    // Launch step 1: read file and write buffer to a pipe
    pthread_t tid1;
    pthread_create(&tid1, NULL, step, (void *) &step1);

    // Pipes to link steps 2 and 3
    Pipes step2Pipes = pipeFiles();
    // Prepare second step parameters
    StepParams step2 = {
            .writer = step2Pipes.writer,
            .reader = step1Pipes.reader,
            .stepFunc = &toLower};
    // Launch step 2: bridge/process buffer (toLower)
    pthread_t tid2;
    pthread_create(&tid2, NULL, step, (void *) &step2);

    // Prepare third step parameters
    StepParams step3 = {.writer = stdout, .reader = step2Pipes.reader};
    // Launch step 3: output buffer
    pthread_t tid3;
    pthread_create(&tid3, NULL, step, (void *) &step3);

    // Wait step1 finish
    pthread_join(tid1, NULL);
    // Close first writer
    fclose(step1Pipes.writer);
    // Wait step2 finish
    pthread_join(tid2, NULL);
    // Close second writer
    fclose(step2Pipes.writer);
    // Wait step3 finish
    pthread_join(tid3, NULL);
    // Close all readers
    fclose(step1Pipes.reader);
    fclose(step2Pipes.reader);
    fclose(file);

    printf("\n");
}