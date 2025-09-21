#include "Utility/Logger.h"

#include <iostream>

Logger* Logger::getInstance()
{
    if (!instance)
        instance = new Logger();

    return instance;
}

Logger::Logger()
{

}

void Logger::log(LogLevel type, std::string message)
{
    if (type >= level)
        std::cout << logLevelToString(type) << ": " << message << std::endl;

}

std::string Logger::logLevelToString(LogLevel type)
{
    switch (level)
    {
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::DEBUG:
            return "DEBUG";
        default:
            return "VERBOSE";
    }
}
