#include "mainwindow.h"
#include "resources/resourcemanager.h"

int main()
{
    ResourseManager::getInstance()->loadResourses();
    MainWindow window{ 1920, 1080, "New Year Chaos" };
    return window.gameLoop();
}
