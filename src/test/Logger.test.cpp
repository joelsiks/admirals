#include <chrono>
#include <ctime>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <thread>

#include "Logger.hpp"

using namespace admirals;

int main(int argc, char *argv[]) {

    // Logger *logWriter = new Logger();

    // std::this_thread::sleep_for(std::chrono::seconds(2));
    Logger::ClearLogFile();
    Logger::LogMessage("test string");
    Logger::CatchError("test", Logger::ErrorCase::NoObjectFound);

    return EXIT_SUCCESS;
}