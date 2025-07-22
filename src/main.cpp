#include "windowing/Mainwindow.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

int main()
{
    MainWindow* window = new MainWindow();
    if (window->isAlive())
        window->exec();

    return 0;
}
