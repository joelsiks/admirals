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
        DataMissing = 103,
        NetworkConnectionLost = 201
    };
    enum WarningCase { MismatchOfValues = 101, WrongIdentifiers = 102 };

private:
    Logger(){};
    static std::string GetCurrentTime() {
        const std::time_t time = std::time(nullptr);
        std::string time_string(std::asctime(std::localtime(&time)));
        return time_string;
    }

    static std::string GetErrorMessage(ErrorCase ec) {
        switch (ec) {
        case ValueOutOfBound:
            return "Error:\n  Value out of bounds";
        case NoObjectFound:
            return "Error:\n  Object not found";
        case DataMissing:
            return "Error:\n  Data missing";
        case NetworkConnectionLost:
            return "Error:\n  Connection lost";
        default: // code 0
            return "unknown error code.";
        }
    }

    static std::string GetWarningMessage(WarningCase ec) {
        switch (ec) {
        case MismatchOfValues:
            return "Warning:\n  Possible mismatch of varaiables";
        case WrongIdentifiers:
            return "Warning:\n  Wrong Inditifiers";
        default: // code 0
            return "unknown warning code.";
        }
    }

    static void AppendToLogFile(const std::string &text) {
        std::ofstream file1;
        file1.open("logFile.txt", std::ios::app);
        file1 << GetCurrentTime();
        file1 << text + "\n\n";
        file1.close();
    }

public:
    static void ClearLogFile() {
        std::ofstream file1;
        file1.open("logFile.txt");
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

    static void CatchWarning(const std::string &logString, WarningCase ec) {
        const std::string text = "\n" + GetWarningMessage(ec) + "\n" +
                                 "---Unique log message: \n" + logString +
                                 "\n" + "---End of Message \n";
        AppendToLogFile(text);
    }
};

} // namespace admirals