#include "MutexFactorialFunction.h"

int main() {
	MutexFunction* function = new MutexFactorialFunction();
	int result = function->calculateResult(6);
	cout << endl << result;
	delete function;
}

//#include "QueueFactorialFunction.h"
//
//int main() {
//    QueueFunction* queueFunction = new QueueFactorialFunction();
//    std::cout << queueFunction->calculateResult(5);
//    delete queueFunction;
//}

