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

    /*!
     * \brief Singleton pattern function for getting the one instance of the logger that exists.
     *        Creating it if it doesn't already exist.
     * \return Returns the singular log instance for the program.
     */
    static Logger* getInstance();

    /*!
     * \brief Sets the log level of the program.
     *        ERROR - Only logs errors
     *        WARNING - Logs errors and warnings
     *        DEBUG - Logs errors, warnings, and some key information for debugging
     *        VERBOSE - Logs errors, warnings, and a lot of information for debugging (only use when really needed, it's a lot of output)
     * \param logLevel
     */
    static void setLogLevel(LogLevel logLevel);

    /*!
     * \brief Logs the given message to the standard output if the message should be included
     *        based on the current log level.
     * \param type - The log level of this particular message.
     * \param message - The information to log.
     */
    void log(LogLevel type, std::string message);

private:
    inline static Logger* instance = nullptr;

    Logger();

    /*!
     * \brief Gives the string representation of the log level
     * \param type - LogLevel to get the string for
     * \return std::string representing the passed log level
     */
    static std::string logLevelToString(LogLevel type);
};

#endif // LOGGER_H
