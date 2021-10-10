#include "MutexFactorialFunction.h"

void MutexFactorialFunction::functionCalculation(long number)
{
	factorialCalculation(number);
}

inline void MutexFactorialFunction::factorialCalculation(long number) {
	for (; currentArgument <= number; currentArgument++) {

		std::unique_lock<std::mutex> lock(calculationMutex);

		currentFunctionValue *= currentArgument;
		notifiedResultListener = true;

		resCalculated.notify_all();

		while (!isLogPrinted) {
			logPrinted.wait(lock);
		}
		isLogPrinted = false;
	}
	done = true;

	resCalculated.notify_all();
}