#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usermap.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    UserMap* getMap();
    bool isSearchGoing();
    void setSearch();
    void unsetSearch();
private:
    bool isSearch;
    void setupMatrix();
    void generateMap(int, int);
    void reGenerateMap(int, int);
    void destroyMap();

    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QPushButton *searchButton;
    QGraphicsScene *scene;
    UserMap* m_map;
};
#endif // MAINWINDOW_H
