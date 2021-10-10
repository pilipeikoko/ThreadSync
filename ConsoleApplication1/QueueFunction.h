#pragma once

#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>
#include <memory>
#include <queue>
#include "Point.h"

class QueueFunction {
protected:
	mutex outputMutex;
	mutex valueOutMutex;
	mutex queueMutex;

	condition_variable resOutput;
	condition_variable loggerLogged;
	condition_variable valueCheck;

	bool loggerLoggedBool;
	bool notifedLogger;
	bool done;
	bool newValue;

	long currentFunctionValue;
	long currentArgument;

	fstream functionResult;
	queue<long> values;

	void initFile(char filename[]);
	void writeFunctionResult(char filename[], string result);

	virtual void resultListener();
	virtual void logWriter();
	virtual void functionCalculation(long number) = 0;

public:
	QueueFunction();
	virtual int calculateResult(int number = 5);

};