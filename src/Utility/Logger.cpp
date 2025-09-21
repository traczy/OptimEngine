#include "Utility/Logger.h"

#include <iostream>

Logger* Logger::getInstance()
{
    if (!instance)
        instance = new Logger();

    return instance;
}

void Logger::setLogLevel(LogLevel logLevel)
{
    std::cout << "Setting log level to " << logLevelToString(logLevel) << std::endl;
    Logger::level = logLevel;
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
    switch (type)
    {
        case LogLevel::ERROR:
        {
            return "ERROR";
        }
        case LogLevel::WARNING:
        {
            return "WARNING";
        }
        case LogLevel::DEBUG:
        {
            return "DEBUG";
        }
        default:
        {
            return "VERBOSE";
        }
    }
}
