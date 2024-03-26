#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QThread>
#include <QRandomGenerator>
#include "usermap.h"
#include "pathfinding.h"
// #include "threadwrapper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    MainWindow w;

    PathSearch* pS = new PathSearch();
    pS->setGraph(w.getMap());
    pS->setStart(QPoint(0,0));


    QThread * thread = new QThread();
    pS->moveToThread(thread);
    QObject::connect(thread, &QThread::started, pS, &PathSearch::bFS);
    QObject::connect(thread, &QThread::started, &w, &MainWindow::setSearch);
    QObject::connect(thread, &QThread::finished, pS, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, &w, &MainWindow::unsetSearch);
    QObject::connect(pS, &PathSearch::pathFound, thread, &QThread::quit);
    thread->start();


    w.show();
    return a.exec();
}
