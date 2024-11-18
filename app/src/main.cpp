#include "mainwindow.h"

int main()
{
    MainWindow window{ 800, 600, "New Year Chaos" };
    return window.gameLoop();
}
