#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

struct ThreadData {
    int leftBorder;
    int rightBorder;
    double result;
};

void* threadFunc(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double localSum = 0;
    for(int i = data->leftBorder; i < data->rightBorder; ++i) {
        localSum += 1.0 / i;
    }
      
    data->result = localSum;
    return NULL;
}

int main(int argc, char** argv) {
    int threadCount, N;
    printf("Please, enter threads count and N:\n");
    int res = scanf("%d %d", &threadCount, &N);
    assert(res == 2);

    pthread_t* threads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    ThreadData* threadData = (ThreadData*)malloc(threadCount * sizeof(ThreadData));

    /* init threads  */
    for(int i = 0; i < threadCount; ++i) {
        threadData[i].leftBorder  = N * i / threadCount + 1;
        threadData[i].rightBorder = N * (i + 1) / threadCount + 1;
        if(threadData[i].rightBorder > N) {
            threadData[i].rightBorder = N + 1; 
        }        

        if((pthread_create(&threads[i], NULL, threadFunc, &threadData[i]))) {
            fprintf(stderr, "error: pthread_create\n");
            return EXIT_FAILURE;
        }
    }

    double ans = 0;
    /* block until all threads complete */
    for(int i = 0; i< threadCount; ++i) {
        pthread_join(threads[i], NULL);
        ans += threadData[i].result;
    }

    printf("ans = %f", ans);

    free(threads);
    free(threadData);
}


