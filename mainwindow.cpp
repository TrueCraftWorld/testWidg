
#include "usermap.h"

#include "visualtile.h"
#include "mainwindow.h"
#include "visualmap.h"

#include <QHBoxLayout>
#include <QSplitter>

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
    unsigned width = 500;
    unsigned height = 500;

    m_map = new UserMap(width, height);
    bool tmp;

    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            tmp = m_map->tileAt(j + width*i)->isWall();
            QColor color = tmp ? Qt::black : Qt::gray;
            VisualTile *item = new VisualTile();
            item->setColor(color);
            item->setCoords(j, i);
            // std::cout  << " N=" << (j + width*i);
            // std::cout  <<((tmp == true) ? "X/" : " /");
            item->setPos(QPointF((j+1)*50, (i+1)*50));

            QObject::connect(m_map->tileAt(j,i), &Tile::stateChanged, item, [=](){
                item->setColor(m_map->tileAt(j,i)->getState() == Tile::States::PATH ? Qt::green : Qt::gray);
            });

            scene->addItem(static_cast<QGraphicsObject *>(item));
        }
        // for (unsigned j = 0; j < width; ++j) std::cout << "__";
        // std::cout << std::endl;
    }
}

UserMap *MainWindow::getMap()
{
    return m_map;
}
