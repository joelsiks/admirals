#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace admirals {

class Logger {

public:
    enum ErrorCase {
        ValueOutOfBound = 101,
        NoObjectFound = 102,
        NetworkConnectionLost = 201
    };

private:
    static constexpr std::string filename = "logFile.txt";

    Logger(){};

    static std::string GetCurrentTime() {
        const std::time_t time = std::time(nullptr);
        std::string time_string(std::asctime(std::localtime(&time)));
        return time_string;
    }

    static std::string GetErrorMessage(ErrorCase ec) {
        switch (ec) {
        case ValueOutOfBound:
            return "Engine Error: \n    Value out of bounds";
        case NoObjectFound:
            return "Engine Error: \n    Object not found";
        case NetworkConnectionLost:
            return "Network error:\n    Connection lost";
        default: // code 0
            return "unknown error code.";
        }
    }

    static void AppendToLogFile(const std::string &text) {
        std::ofstream file1;
        file1.open(filename, std::ios::app);
        file1 << GetCurrentTime();
        file1 << text + "\n\n";
        file1.close();
    }

public:
    static void ClearLogFile() {
        std::ofstream file1;
        file1.open(filename);
        file1.close();
    }

    static void LogMessage(const std::string &logString) {
        AppendToLogFile(logString);
    }

    static void CatchError(const std::string &logString, ErrorCase ec) {
        const std::string text = "\n" + GetErrorMessage(ec) + "\n" +
                                 "---Unique log message: \n" + logString +
                                 "\n" + "---End of Message \n";
        AppendToLogFile(text);
    }
};

} // namespace admirals