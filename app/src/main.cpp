#include "mainwindow.h"
#include "resources/resourcemanager.h"

int main()
{
    ResourseManager::getInstance()->loadResourses();
    MainWindow window{ 1150, 800, "New Year Chaos" };
    return window.gameLoop();
}
