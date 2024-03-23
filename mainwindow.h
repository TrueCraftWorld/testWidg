#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usermap.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    UserMap* getMap();
private:
    void setupMatrix();
    void generateMap();

    QGraphicsScene *scene;
    UserMap* m_map;
};
#endif // MAINWINDOW_H
