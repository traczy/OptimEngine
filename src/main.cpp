#include "Windowing/Mainwindow.h"
#include "Utility/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    // Set log level based on arguments given
    if (argc > 2)
    {
        std::cerr << "too many arguments given to program. Give at most 2 arguments: <program> <log_level (0-3)>. Running with default log level 'ERROR'." << std::endl;
    }
    else if (argc == 2)
    {
        std::string logArg = argv[1];
        try
        {
            int logLevel = std::stoi(logArg);
            if (logLevel > 3)
                std::cerr << "Log level given in argument does not exist. Defaulting to 'ERROR' log level." << std::endl;
            else
                Logger::setLogLevel(static_cast<LogLevel>(logLevel));
        }
        catch (std::exception& e)
        {
            std::cerr << "Parsing log argument failed. " << e.what() << std::endl << "Valid values are integers in the range 0-3. Defaulting to 'ERROR' log level." << std::endl;
        }
    }
    else
    {
        std::cout << "No logging level given. Using default logging level 'ERROR'." << std::endl;
    }

    MainWindow* window = new MainWindow();
    if (window->isAlive())
        window->exec();

    return 0;
}
