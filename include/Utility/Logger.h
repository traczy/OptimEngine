#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum LogLevel
{
    ERROR = 0,
    WARNING,
    DEBUG,
    VERBOSE
};

class Logger
{
public:
    inline static LogLevel level = LogLevel::ERROR;

    static Logger* getInstance();

    void log(LogLevel type, std::string message);

private:
    inline static Logger* instance = nullptr;

    Logger();

    std::string logLevelToString(LogLevel type);
};

#endif // LOGGER_H
