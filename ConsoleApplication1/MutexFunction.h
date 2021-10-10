#pragma once
#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>
#include <memory>
#include "Point.h"

using namespace std;

class MutexFunction {
protected:
	mutex outputMutex;
	mutex calculationMutex;
	condition_variable resCalculated;
	condition_variable logPrinted;

	bool notifiedResultListener;
	bool notifedLogger;
	bool isLogPrinted;
	bool done;

	long currentFunctionValue;
	long currentArgument;

	fstream functionResultWritter;

	static string logFilename;
	static string resultFilename;

	void initFile(char filename[]);
	void writeFunctionResult(char filename[], std::string result);

	virtual void resultListener();
	virtual void logWriter();
	virtual void functionCalculation(long number) = 0;

public:
	MutexFunction();
	virtual int calculateResult(int number = 5);
};