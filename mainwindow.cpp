
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
    generateMap(80,80);

    VisualMap *v_map = new VisualMap("Please work");
    v_map->tileMap()->setScene(scene);

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
        // scene->
        v_map->updateGeometry();
    });
}



void MainWindow::generateMap(int width, int height)
{
    // unsigned width = 80;
    // unsigned height = 80;
    for (auto item : scene->items()) {
        scene->removeItem(item);
    }
    m_map = new UserMap(this);
    m_map->setSize(QSize(width, height));
    m_map->create();
    // setMouseTracking(true);
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
                item->setColor(m_map->tileAt(column,row)->getState() == Tile::States::PATH ? Qt::green : Qt::gray);
                item->update();
            });
            QObject::connect(item, &VisualTile::mouseEntered, m_map, &UserMap::setGoal);
            QObject::connect(item, &VisualTile::mouseLeaved, m_map, &UserMap::unsetGoal);
            QObject::connect(item, &VisualTile::mouseReleased, m_map, &UserMap::resetStart);
            scene->addItem(static_cast<QGraphicsObject *>(item));
        }
    }
}

void MainWindow::reGenerateMap(int width, int height)
{
    for (auto item : scene->items()) {
        scene->removeItem(item);
    }
    m_map->setSize(QSize(width, height));
    m_map->create();
    // setMouseTracking(true);
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
                item->setColor(m_map->tileAt(column,row)->getState() == Tile::States::PATH ? Qt::green : Qt::gray);
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
