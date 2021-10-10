#pragma once
#include "QueueFunction.h"

class QueueFactorialFunction : public QueueFunction {
private:
	void functionCalculation(long number) override;
	void factorialCalculation(long n);
};