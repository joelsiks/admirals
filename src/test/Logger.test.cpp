#include <chrono>
#include <ctime>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <thread>

#include "Logger.hpp"

using namespace admirals;

int main(int argc, char *argv[]) {

    Logger *logWriter = new Logger();

    logWriter->catchError("error found");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    logWriter->catchError("new line",
                          std::error_code(1337, std::system_category()));
    logWriter->catchError(std::error_code(101, std::system_category()));

    return EXIT_SUCCESS;
}