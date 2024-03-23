#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QThread>
#include "usermap.h"
#include "pathfinding.h"
// #include "threadwrapper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    MainWindow w;

    // unsigned width = 20;
    // unsigned height = 20;
    // UserMap some_map = UserMap(width, height);
    // bool tmp;

    // for (unsigned i = 0; i < height; ++i) {
    //     for (unsigned j = 0; j < width; ++j) {
    //         tmp = some_map.m_tiles.at(j + width*i)->isWall();

    //         // std::cout  << " N=" << (j + width*i);
    //         std::cout  <<((tmp == true) ? "X/" : " /");
    //     }
    //     // for (unsigned j = 0; j < width; ++j) std::cout << "__";
    //     std::cout << std::endl;
    // }

    // if (PathSearch::breadthFirstSearch(some_map, Point{0,0}, Point{6,0})) {
    //     PathSearch::printPath(some_map,  Point{6,0});
    // } else {
    //     std::cout << "NO PATH!" << std::endl;
    // }

    // Controller * ctrl = new Controller();
    // ctrl->operate(some_map, Point{0,0}, Point{6,0});
    // QObject::connect(ctrl, &Controller::results, [=](bool res) mutable {
    //     if (res) {
    //         PathSearch::printPath(some_map,  Point{6,0});
    //     } else {
    //         std::cout << "NO PATH!" << std::endl;
    //     }
    // });

    PathSearch* pS = new PathSearch();
    pS->setGraph(w.getMap());
    pS->setStart(QPoint(0,0));
    pS->setGoal(QPoint(250,400));

    QThread * thread = new QThread();
    pS->moveToThread(thread);
    QObject::connect(thread, &QThread::started, pS, &PathSearch::bFS);
    QObject::connect(thread, &QThread::finished, pS, &QObject::deleteLater);
    QObject::connect(pS, &PathSearch::pathFound, [&](bool result) mutable {
        if (result) PathSearch::printPath(w.getMap(), QPoint(6,0));
        else std::cout << "NO PATH!" << std::endl;
    });
    QObject::connect(pS, &PathSearch::pathFound, thread, &QThread::quit);
    thread->start();


    // if (pS->bFS()) {
    //     PathSearch::printPath(some_map, QPoint(6,0));
    // } else {
    //     std::cout << "NO PATH!" << std::endl;
    // }


    w.show();
    return a.exec();
}
