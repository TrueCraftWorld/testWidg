#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usermap.h"
#include "visualmap.h"

#include <QWidget>
#include <QObject>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
class QPushButton;
class QLineEdit;
class QIntValidator;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    UserMap* getMap();
    bool isMapRegenON();
    void setMapRegen(bool);

private:

    void generateMap(int, int);
    void reGenerateMap(int, int);
    void createVisual();
    void closeEvent(QCloseEvent *event) override;

    bool isMapRegen;
    QGraphicsScene *scene;
    UserMap* m_map;
    MapView *v_map;
    QPushButton *searchButton;
    QIntValidator *validator;
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
};

#endif // MAINWINDOW_H
