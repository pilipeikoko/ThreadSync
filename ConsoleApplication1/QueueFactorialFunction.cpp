#include "QueueFactorialFunction.h"

void QueueFactorialFunction::functionCalculation(long number)
{
	factorialCalculation(number);
}

void QueueFactorialFunction::factorialCalculation(long n)
{
    for (; currentArgument <= n; currentArgument++) {
        currentFunctionValue *= currentArgument;
        {
            std::unique_lock<std::mutex> locker(queueMutex);
            values.push(currentFunctionValue);
            newValue = true;
            valueCheck.notify_one();
        }
    }
    done = true;
    newValue = true;
    valueCheck.notify_all();
}
