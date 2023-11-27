#pragma once

#include <ctime>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace admirals {

class Logger {
private:
    std::ofstream textlog_m;
    const std::string filename_m = "logFile.txt";
    const std::time_t startTime_m = std::time(nullptr);

    const std::string timeSinceStart() {
        const std::time_t currentTime = std::time(nullptr);
        long long time_taken = currentTime - startTime_m;
        long sec = time_taken % 60;
        long min = (time_taken / 60 ) % 60;
        long hour = (time_taken / 3600) % 60;
        std::string return_str = std::to_string(hour) 
                        + ":" + std::to_string(min) 
                        + ":" + std::to_string(sec);
        return return_str;
    }
public:
    Logger(){
        textlog_m.open(filename_m);
        textlog_m.close();
    };

    void catchError(int ec) {
        catchError(std::error_code(ec, std::system_category()));
    }
    void catchError(const std::string &logString, int ec) {
        catchError(logString, std::error_code(ec, std::system_category()));
    }
    void catchError(std::error_code ec) {
        catchError("NO MESSAGE", ec);
    }
    void catchError(const std::string &logString) {
        catchError(logString, std::error_code()); //error code 0
    }
    void catchError(const std::string &logString, std::error_code ec) {
        textlog_m.open(filename_m, std::ios::app);
        textlog_m << std::asctime(std::localtime(&startTime_m))
                << "time since start: " + (timeSinceStart()) + "\n"
                << getErrorMessage(ec) + "\n"
                << "---Unique log message: \n"
                << logString + "\n"
                << "---End of Message \n"
                << std::filesystem::current_path(ec)
                << ("\n\n");

        textlog_m.close();
    }

    std::string getErrorMessage(std::error_code ec) {
        switch (ec.value())
        {
        case 1337:
            return "One code paragraph was too elite.";
        case 101:
            return "Engine Error: \n    Value out of bounds";
        case 102:
            return "Engine Error: \n    Object not found";
        case 201:
            return "Network error:\n    Connection lost";
        default: //code 0
            return "unknown error code.";
        }
    }

};

} // namespace admirals