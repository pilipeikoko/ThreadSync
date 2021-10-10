#include "QueueFunction.h"

void QueueFunction::initFile(char filename[])
{
	functionResult.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
	functionResult.close();
}

void QueueFunction::writeFunctionResult(char filename[], std::string result)
{
	functionResult.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	functionResult << result << "\n";
	functionResult.close();
}

void QueueFunction::resultListener()
{
    const char filename[] = "../results";
    {
        std::unique_lock<std::mutex> outLock(outputMutex);
        initFile(const_cast<char*>(filename));
    }

    while (!done) {
        std::unique_lock<std::mutex> lock(queueMutex);
        while (!newValue) {

            valueCheck.wait(lock);
        }
        newValue = false;
        while (!values.empty()) {
            {
                std::unique_lock<std::mutex> outLock(outputMutex);
                writeFunctionResult(const_cast<char*>(filename), std::to_string(values.front()));

            }
            // вывели в файл и теперь ждём, пока отработает логгер. Как только он отработал - идём дальше и
            // так пока не закончится очередь и !done.
            notifedLogger = true;
            resOutput.notify_one();

            while (!loggerLoggedBool) {
                loggerLogged.wait(lock);
            }
            loggerLoggedBool = false;
        }
    }
}

void QueueFunction::logWriter()
{
    const char filename[] = "../log";
    {
        std::unique_lock<std::mutex> lock(outputMutex);
        initFile(const_cast<char*>(filename));
    }
    bool first_iteration = true;
    // тут фишка в том, чтобы он не выходил из цикла, когда будет done = true, т.к. это будет преждевременно
    // поэтому добавляем условие пустой очереди. Но на первой итерации очередь пустая, а нам нужно зайти в цикл
    // поэтому добавляем булеву переменную первой итерации (я сам в ахуе).
    while (!done || (!values.empty() || first_iteration)) {
        first_iteration = false;
        std::unique_lock<std::mutex> lock(valueOutMutex);
        while (!notifedLogger) {
            resOutput.wait(lock);
        }
        notifedLogger = false;
        time_t now = time(0);
        std::string initialized_time = ctime(&now);
        long value;

        {
            // работу с очередью умещаем здесь (типа рекомендуется минимизировать блоки с локами)
            std::unique_lock<std::mutex> queueLock(queueMutex);
            value = values.front();
            values.pop();
        }

        std::shared_ptr<Point> current_point(
            new Point(currentFunctionValue - 1, value, initialized_time));

        {
            std::unique_lock<std::mutex> outLock(outputMutex);
            writeFunctionResult(const_cast<char*>(filename), current_point->logValue());
        }
        loggerLoggedBool = true;
        loggerLogged.notify_one();

    }
    std::cout << "LOG EXIT\n";
}

QueueFunction::QueueFunction()
{
    loggerLoggedBool = false;
    notifedLogger = false;
    done = false;
    newValue = false;

    currentFunctionValue = 1;
    currentArgument = 1;
}

int QueueFunction::calculateResult(int number)
{
    std::thread result(&QueueFunction::resultListener,this);
    std::thread log(&QueueFunction::logWriter,this);
    // таймаут чтобы логгеры успели встать на ожидание вычисления
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    std::thread factorial(&QueueFunction::functionCalculation,this, number);

    factorial.join();
    result.join();
    log.join();

    return currentFunctionValue;
}
