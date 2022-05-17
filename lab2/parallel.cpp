#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include "helper.hpp"

sem_t semGlobalSum;
sem_t semGlobalStack;
sem_t semTaskPresent;

int numberOfActiveProcs = 0;
unsigned int maxGSelements = 10, maxLSelements = 5;
double integral = 0;
std::stack<SUBTASK_RECORD> globalStack;

struct ThreadData {
    int threadsCount;
    TASK taskToSolve;
    double eps;
};

void *calculateIntegral(void* data) {
    ThreadData threadData = *(ThreadData*)data;


	std::stack<SUBTASK_RECORD> localStack;
	double J = 0;
	double A = threadData.taskToSolve.a;
	double B = threadData.taskToSolve.b;
	auto&& f = threadData.taskToSolve.func;

	while (1) {
		sem_wait(&semTaskPresent);
		sem_wait(&semGlobalStack);

		SUBTASK_RECORD tmpRecordAB = globalStack.top();
		double A = tmpRecordAB.a;
		double fA = f(A);
		double B = tmpRecordAB.b;
		double fB = f(B);	
		double sAB = tmpRecordAB.sum;
		globalStack.pop();

		if (!globalStack.empty())
			sem_post(&semTaskPresent);
		if (A <= B)
			numberOfActiveProcs++;
			sem_post(&semGlobalStack);
		if (A > B) break;		

		while (1) {
			double C = (A + B) / 2;
			double fC = f(C);
			double sAC = (fA + fC) * (C - A) / 2;
			double sCB = (fC + fB) * (B - C) / 2;
			double sACB = sAC + sCB;
			if (abs(sAB - sACB) < threadData.eps * abs(sACB)) {
				J += sACB;
	
				if (localStack.empty())
					break;
				else {
					tmpRecordAB = localStack.top();
					A = tmpRecordAB.a;
					fA = threadData.taskToSolve.func(A);
					B = tmpRecordAB.b;
					fB = threadData.taskToSolve.func(B);
					sAB = tmpRecordAB.sum;
					localStack.pop();
				}
			} else {
				SUBTASK_RECORD tmpRecordAC = { A, C, sAC };
				localStack.push(tmpRecordAC);
				A = C;
				fA=fC;
				sAB = sCB;
			}

			if ((localStack.size() > maxLSelements) && 
				(globalStack.empty())) {
				sem_wait(&semGlobalStack);
				if (globalStack.empty()) {
					sem_post(&semTaskPresent);
				}
				while ((localStack.size() > 1)
						&& (globalStack.size() < maxGSelements)) {
					SUBTASK_RECORD tmpRecord = localStack.top();
					localStack.pop();
					globalStack.push(tmpRecord);
				}
				sem_post(&semGlobalStack);
			}
		}

		sem_wait(&semGlobalStack);
		numberOfActiveProcs--;
		if ((!numberOfActiveProcs) && (globalStack.empty())) {
			for (int i = 0; i < threadData.threadsCount; i++) {
				SUBTASK_RECORD terminalRecord = { 2, 1, 0 };
				globalStack.push(terminalRecord);
			}
			sem_post(&semTaskPresent);
		}
		sem_post(&semGlobalStack);
	}

	sem_wait(&semGlobalSum);
	integral += J;
	sem_post(&semGlobalSum);
	return 0;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << "Invalid command line arguments" << std::endl;
        return 0;
    }

    double eps = std::stod(argv[2]);
    std::size_t threadCount = std::stoi(argv[1]);

    Timer time;

    const TASK taskToSolve;
	double A = taskToSolve.a;
	double B = taskToSolve.b;
	auto&& f = taskToSolve.func;

	sem_init(&semGlobalSum, 0, 1);
	sem_init(&semGlobalStack, 0, 1);
	sem_init(&semTaskPresent, 0, 0);

	double fA = f(A);
	double fB = f(B);
	SUBTASK_RECORD tmpRecordAB = { A, B, (fA + fB) * (B - A) / 2. };
	globalStack.push(tmpRecordAB);
	sem_post(&semTaskPresent);

    std::vector<pthread_t> threads(threadCount);
    std::vector<ThreadData> threadsData(threadCount);
    for(int i = 0; i < threadCount; i++) {
        threadsData[i].threadsCount = threadCount;
        threadsData[i].eps = eps;
        threadsData[i].taskToSolve = taskToSolve;
    }

    for (int i = 0; i < threadCount; i++) {
		pthread_create(&(threads[i]), NULL, calculateIntegral, &threadsData[i] );
    }

	for (int i = 0; i < threadCount; i++) {
		pthread_join(threads[i], NULL);
    }

    auto resTime = time.elapsed();

    std::cout <<  "Result: " << integral << std::endl;
    std::cout <<  "Time: " << resTime << std::endl;

	return 0;
}