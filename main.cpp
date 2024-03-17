#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "usermap.h"
#include "pathfinding.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    unsigned width = 25;
    unsigned height = 10;
    UserMap some_map = UserMap(width, height);
    bool tmp;
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            tmp = some_map.m_tiles[j + width*i]->isWall();
            // std::cout  << " N=" << (j + width*i);
            std::cout  <<((tmp == true) ? "1 " : "0 ");
        }
         std::cout << std::endl;
    }

    if (PathSearch::breadthFirstSearch(some_map, Point{0,0}, Point{6,0})) {
        PathSearch::printPath(some_map,  Point{6,0});
    }

    // w.show();
    return a.exec();
}
