#include "windowing/Mainwindow.h"

#include <iostream>

int main()
{
    MainWindow* window = new MainWindow();
    if (window->isAlive())
        window->exec();

    return 0;
}
