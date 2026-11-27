#pragma once

#include <string>

//Holds the last error that occured
inline std::string errLog;

//Clears and pushes an new error to the error log
inline void pushErr(std::string ERROR) {
    errLog = "";
    errLog = ERROR;
}

//Returns and clears the error log.
inline std::string pullErr() {
    std::string error = errLog;
    errLog = "";
    return error;
}