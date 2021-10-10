#include "MutexFactorialFunction.h"

int main() {
	Function* function = new MutexFactorialFunction();
	int result = function->calculateResult(6);
	cout << endl << result;
	delete function;
}
