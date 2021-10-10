#include "MutexFunction.h"

MutexFunction::MutexFunction() {
	notifiedResultListener = false;
	notifedLogger = false;
	isLogPrinted = false;
	done = false;

	currentFunctionValue = 1;
	currentArgument = 1;
}

inline void MutexFunction::initFile(char filename[])
{
	functionResultWritter.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
	functionResultWritter.close();
}

inline void MutexFunction::writeFunctionResult(char filename[], std::string result)
{
	functionResultWritter.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	functionResultWritter << result << "\n";
	functionResultWritter.close();
}

inline void MutexFunction::resultListener() {
	const char filename[] = "../results";
	{
		std::unique_lock<std::mutex> outLock(outputMutex);
		initFile(const_cast<char*>(filename));
	}
	std::unique_lock<std::mutex> lock(calculationMutex);

	while (!done) {
		while (!notifiedResultListener) {
			resCalculated.wait(lock);
			if (done) {
				return;
			}
		}
		{
			std::unique_lock<std::mutex> outLock(outputMutex);
			writeFunctionResult(const_cast<char*>(filename), std::to_string(currentFunctionValue));
		}
		notifiedResultListener = false;
		notifedLogger = true;
		resCalculated.notify_all();
	}
}

inline void MutexFunction::logWriter() {
	const char filename[] = "../log";
	{
		std::unique_lock<std::mutex> lock(outputMutex);
		initFile(const_cast<char*>(filename));
	}
	while (!done) {
		std::unique_lock<std::mutex> lock(calculationMutex);
		while (!notifedLogger) {
			resCalculated.wait(lock);
			if (done) {
				return;
			}
		}

		notifedLogger = false;
		time_t now = time(0);
		std::string initialized_time = ctime(&now);
		std::shared_ptr<Point> current_point(
			new Point(currentArgument - 1, currentFunctionValue, initialized_time));
		{
			std::unique_lock<std::mutex> outLock(outputMutex);
			writeFunctionResult(const_cast<char*>(filename), current_point->logValue());
		}
		logPrinted.notify_one();
		isLogPrinted = true;
	}
}

inline int MutexFunction::calculateResult(int number)
{
	std::thread result(&MutexFunction::resultListener, this);
	std::thread log(&MutexFunction::logWriter, this);
	std::thread function(&MutexFunction::functionCalculation, this, number);


	function.join();
	result.join();
	log.join();

	std::cout << "completed";

	return currentFunctionValue;
}
