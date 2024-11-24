#include "mainwindow.h"
#include "resources/resourceManager.h"

int main()
{
    ResourseManager::getInstance()->loadResourses();
    MainWindow window{ 800, 600, "New Year Chaos" };
    return window.gameLoop();
}
