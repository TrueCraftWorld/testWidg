
#include "usermap.h"
#include <iostream>
#include "visualtile.h"
#include "mainwindow.h"
#include "visualmap.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QSize>
#include <QGraphicsObject>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), scene(new QGraphicsScene(this))

{
    generateMap();

    VisualMap *v_map = new VisualMap("Please work");
    v_map->tileMap()->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(v_map);
    setLayout(layout);
    setWindowTitle(tr("Path Test"));
}



void MainWindow::generateMap()
{
    unsigned width = 160;
    unsigned height = 90;

    m_map = new UserMap(this);
    m_map->setSize(QSize(width, height));
    m_map->create();
    // setMouseTracking(true);
    bool tmp;

    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            tmp = m_map->tileAt(j + width*i)->isWall();
            QColor color = tmp ? Qt::black : Qt::gray;
            VisualTile *item = new VisualTile();
            item->setColor(color);
            item->setCoords(j, i);
            item->setPos(QPointF((j+1)*50, (i+1)*50));

            QObject::connect(m_map->tileAt(j,i), &Tile::stateChanged, item, [=](){
                item->setColor(m_map->tileAt(j,i)->getState() == Tile::States::PATH ? Qt::green : Qt::gray);
                item->update();
            });
            QObject::connect(item, &VisualTile::mouseEntered, m_map, &UserMap::setGoal);
            QObject::connect(item, &VisualTile::mouseLeaved, m_map, &UserMap::unsetGoal);
            scene->addItem(static_cast<QGraphicsObject *>(item));
        }
    }
}

UserMap *MainWindow::getMap()
{
    return m_map;
}

bool MainWindow::isSearchGoing()
{
    return isSearch;
}

void MainWindow::setSearch()
{
    isSearch = true;
}

void MainWindow::unsetSearch()
{
    isSearch = false;
}
