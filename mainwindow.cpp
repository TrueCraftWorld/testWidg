
#include "usermap.h"
#include <iostream>
#include "visualtile.h"
#include "mainwindow.h"
#include "visualmap.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSize>
#include <QGraphicsObject>
#include <QPushButton>
#include <QLineEdit>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), scene(new QGraphicsScene(this))

{
    // generateMap(80,80);
    // m_map = new UserMap(this);
    m_map = new UserMap();

    VisualMap *v_map = new VisualMap("Please work");
    v_map->tileMap()->setScene(scene);
    v_map->tileMap()->setBaseSize(QSize(600,600));
    v_map->tileMap()->setMinimumHeight(600);
    v_map->tileMap()->setMinimumWidth(600);

    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *layoutV = new QVBoxLayout;
    searchButton = new QPushButton;
    searchButton->setText(tr("Generate"));
    searchButton->setCheckable(true);
    searchButton->setChecked(true);


    widthEdit = new QLineEdit;
    widthEdit->setInputMask(QString("ddddd"));
    widthEdit->setPlaceholderText(QString("Width"));
    widthEdit->setCursorPosition(0);
    widthEdit->setText(QString("80"));
    widthEdit->clear();

    heightEdit = new QLineEdit;
    heightEdit->setInputMask(QString("ddddd"));
    heightEdit->setPlaceholderText(QString("Height"));
    heightEdit->setCursorPosition(0);
    heightEdit->setText(QString("80"));
    heightEdit->clear();

    layoutV->addStretch();
    layoutV->addWidget(widthEdit);
    layoutV->addStretch();
    layoutV->addWidget(heightEdit);
    layoutV->addStretch();
    layoutV->addWidget(searchButton);
    layoutV->addStretch();

    layout->addWidget(v_map);
    layout->addStretch();
    layout->addLayout(layoutV);

    setLayout(layout);
    setWindowTitle(tr("Path Test"));

    QObject::connect(searchButton, &QPushButton::clicked,
                     m_map, &UserMap::empty);

    QObject::connect(m_map, &UserMap::emptied,
                     this, [=]() {

        reGenerateMap(widthEdit->displayText().toInt(), heightEdit->displayText().toInt());
        // v_map->tileMap()->resetTransform();
        v_map->tileMap()->scene()->setSceneRect(v_map->tileMap()->scene()->itemsBoundingRect());
        v_map->tileMap()->scene()->update();
        v_map->updateGeometry();
    });
}



void MainWindow::generateMap(int width, int height)
{


    // reGenerateMap(width, height);
}

void MainWindow::reGenerateMap(int width, int height)
{

    scene->clear();

    m_map->setSize(QSize(width, height));
    m_map->create();
    bool tmp;

    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            tmp = m_map->tileAt(column + width*row)->isWall();
            QColor color = tmp ? Qt::black : Qt::gray;
            VisualTile *item = new VisualTile();
            item->setColor(color);
            item->setCoords(column, row);
            item->setPos(QPointF((column+1)*50, (row+1)*50));

            QObject::connect(m_map->tileAt(column,row), &Tile::stateChanged, item, [=](){
                QColor color;
                switch (m_map->tileAt(column,row)->getState()) {
                case Tile::States::PATH:
                    color = Qt::green;
                    break;
                case Tile::States::EMPTY:
                    color = Qt::gray;
                    break;
                case Tile::States::WALL:
                    color = Qt::black;
                    break;
                case Tile::States::START:
                    color = Qt::blue;
                    break;
                case Tile::States::STOP:
                    color = Qt::red;
                    break;
                default:
                    break;
                }
                item->setColor(color);
                item->update();
            });
            QObject::connect(item, &VisualTile::mouseEntered, m_map, &UserMap::setGoal);
            QObject::connect(item, &VisualTile::mouseLeaved, m_map, &UserMap::unsetGoal);
            QObject::connect(item, &VisualTile::mouseReleased, m_map, &UserMap::resetStart);
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
