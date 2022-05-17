#include <iostream>
#include <stack>
#include "helper.hpp"

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cout << "Invalid command line arguments" << std::endl;
        return 0;
    }

    double eps = std::stod(argv[1]);

    Timer time;

    TASK taskToSolve;
	double A = taskToSolve.a;
	double B = taskToSolve.b;
	auto&& f = taskToSolve.func;
	
	std::stack<SUBTASK_RECORD> localStack;

    double J = 0;
	double fA = f(A);
	double fB = f(B);
	double sAB = (fA + fB) * (B - A) / 2.;
	while (1) {
		double C = (A + B) / 2.;

		double fC = f(C);
		double sAC = (fA + fC) * (C - A) / 2.;
		double sCB = (fC + fB) * (B - C) / 2.;
		double sACB = sAC + sCB;
		if (std::abs(sAB - sACB) >= eps * std::abs(sACB)) {
			SUBTASK_RECORD tmpRecordAC = { A, C, sAC };
			localStack.push(tmpRecordAC);
			A = C;
			fA = fC;
			sAB = sCB;
		} else {
			J += sACB;
			if (localStack.empty())
				break;
			else {
				SUBTASK_RECORD tmpRecordAB = localStack.top();
				A = tmpRecordAB.a;
				fA = f(A);
				B = tmpRecordAB.b;
				fB = f(B);
				sAB = tmpRecordAB.sum;
				localStack.pop();
			}
		}
	}
    auto resTime = time.elapsed();

    std::cout <<  "Result: " << J << std::endl;
    std::cout <<  "Time: " << resTime << std::endl;
}
