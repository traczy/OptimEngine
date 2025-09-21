#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum LogLevel
{
    ERROR = 0,
    WARNING = 1,
    DEBUG = 2,
    VERBOSE = 3
};

class Logger
{
public:
    inline static LogLevel level = LogLevel::ERROR;

    static Logger* getInstance();

    static void setLogLevel(LogLevel logLevel);

    void log(LogLevel type, std::string message);

private:
    inline static Logger* instance = nullptr;

    Logger();

    static std::string logLevelToString(LogLevel type);
};

#endif // LOGGER_H
