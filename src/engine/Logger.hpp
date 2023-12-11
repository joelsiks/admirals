#pragma once

#include <chrono>
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
    inline static std::chrono::time_point m_startTime =
        std::chrono::system_clock::now();

private:
    static std::string GetCurrentTime() {
        const std::time_t time = std::time(nullptr);
        std::string time_string(std::asctime(std::localtime(&time)));
        return time_string;
    }

    static std::string GetTimeSinceStart() {
        const std::time_t time =
            std::time(nullptr) -
            std::chrono::system_clock::to_time_t(m_startTime);
        const size_t sec = time % 60;
        const size_t min = (time / 60) % 60;
        const size_t hour = (time / 3600) % 60;
        std::string return_str = std::to_string(hour) + ":" +
                                 std::to_string(min) + ":" +
                                 std::to_string(sec);
        return return_str;
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
            return "Warning:\n  Possible mismatch of variables";
        case WrongIdentifiers:
            return "Warning:\n  Wrong Identifiers";
        default: // code 0
            return "unknown warning code.";
        }
    }

    static void AppendToLogFile(const std::string &text) {
        std::ofstream file1;
        file1.open("logFile.txt", std::ios::app);
        file1 << GetCurrentTime() << GetTimeSinceStart() << "\n";
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
        std::cout << "LogMessage: \n" << logString << "\n";
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