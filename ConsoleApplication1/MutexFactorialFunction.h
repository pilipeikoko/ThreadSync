#pragma once
#include "Function.h"

class MutexFactorialFunction : public Function {
private:
	void functionCalculation(long number) override;
	void factorialCalculation(long n);
};