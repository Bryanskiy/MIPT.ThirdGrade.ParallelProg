#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

struct ThreadData {
    int id;
    int threadCount;
};

void* threadFunc(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("Hello! id: %d, all: %d \n", data->id, data->threadCount);
    return NULL;
}

int main(int argc, char** argv) {
    int threadCount;
    printf("Please, enter threads count:\n");
    int res = scanf("%d", &threadCount);
    assert(res == 1);

    pthread_t* threads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    ThreadData* threadData = (ThreadData*)malloc(threadCount * sizeof(ThreadData));

    /* init threads  */
    for(int i = 0; i < threadCount; ++i) {
        threadData[i].id = i;
        threadData[i].threadCount = threadCount;
        
        if((pthread_create(&threads[i], NULL, threadFunc, &threadData[i]))) {
            fprintf(stderr, "error: pthread_create\n");
            return EXIT_FAILURE;
        }
    }

    /* block until all threads complete */
    for(int i = 0; i< threadCount; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(threadData);
    return EXIT_SUCCESS;
}
