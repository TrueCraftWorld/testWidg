#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "usermap.h"
#include "pathfinding.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    unsigned width = 20;
    unsigned height = 20;
    UserMap some_map = UserMap(width, height);
    bool tmp;

    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            tmp = some_map.m_tiles.at(j + width*i)->isWall();

            // std::cout  << " N=" << (j + width*i);
            std::cout  <<((tmp == true) ? "X/" : " /");
        }
        // for (unsigned j = 0; j < width; ++j) std::cout << "__";
        std::cout << std::endl;
    }

    if (PathSearch::breadthFirstSearch(some_map, Point{0,0}, Point{6,0})) {
        PathSearch::printPath(some_map,  Point{6,0});
    } else {
        std::cout << "NO PATH!" << std::endl;
    }

    w.show();
    return a.exec();
}
