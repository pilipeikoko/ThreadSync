#pragma once
#include "MutexFunction.h"

class MutexFactorialFunction : public MutexFunction {
private:
	void functionCalculation(long number) override;
	void factorialCalculation(long n);
};