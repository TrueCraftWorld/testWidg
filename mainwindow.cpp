
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
            tmp = m_map->m_tiles.at(j + width*i)->isWall();
            QColor color = tmp ? Qt::black : Qt::blue;
            QGraphicsItem *item = new VisualTile(color, j, i, tmp ? States::WALL : States::EMPTY);
            // std::cout  << " N=" << (j + width*i);
            // std::cout  <<((tmp == true) ? "X/" : " /");
            item->setPos(QPointF((j+1)*50, (i+1)*50));
            scene->addItem(item);
        }
        // for (unsigned j = 0; j < width; ++j) std::cout << "__";
        // std::cout << std::endl;
    }
}

UserMap *MainWindow::getMap()
{
    return m_map;
}
