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
#include <QIntValidator>
#include <QThread>
#include <QCoreApplication>
#include <QSettings>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      scene(new QGraphicsScene(this)),
      m_map(new UserMap()),
      v_map(new MapView()),
      searchButton(new QPushButton(this)),
      validator(new QIntValidator(1,999)),
      widthEdit(new QLineEdit(this)),
      heightEdit(new QLineEdit(this))
{
    QSettings settings("CraftWorld", "pathSearch");
    m_map->setParent(this);
    v_map->tileMap()->setScene(scene);
    v_map->tileMap()->setMinimumHeight(450);
    v_map->tileMap()->setMinimumWidth(450);

    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *layoutV = new QVBoxLayout;

    searchButton->setCheckable(false);
    searchButton->setText(tr("Generate"));
    searchButton->setMaximumWidth(150);

    QLabel *widthLabel = new QLabel;
    widthLabel->setText(QString("Width in Tiles"));
    widthEdit->setValidator(validator);
    widthEdit->setPlaceholderText(QString("Max Value - 999"));
    widthEdit->setText(QString(""));
    widthEdit->setMaximumWidth(150);

    QLabel *heightLabel = new QLabel;
    heightLabel->setText(QString("Height in Tiles"));
    heightEdit->setValidator(validator);
    heightEdit->setPlaceholderText(QString("Max Value - 999"));
    heightEdit->setText(QString(""));
    heightEdit->setMaximumWidth(150);

    layoutV->addWidget(widthLabel);
    layoutV->addWidget(widthEdit);
    layoutV->addWidget(heightLabel);
    layoutV->addWidget(heightEdit);
    layoutV->addWidget(searchButton);
    layoutV->addStretch();

    layout->addWidget(v_map);
    layout->addLayout(layoutV);

    widthEdit->move(widthEdit->x(), v_map->y());

    setLayout(layout);
    setWindowTitle(tr("Path Test"));
    this->restoreGeometry(settings.value("geometry").toByteArray());

    QObject::connect(searchButton, &QPushButton::clicked,
                     m_map, &UserMap::empty);

    QObject::connect(m_map, &UserMap::emptied,
                     this, [this]() {
        v_map->zoomReset();
        reGenerateMap(widthEdit->displayText().toInt(), heightEdit->displayText().toInt());
    });
    QObject::connect(m_map, &UserMap::mapReady, this, &MainWindow::createVisual);
}


void MainWindow::reGenerateMap(int width, int height)
{
    QObject::connect(scene, &QObject::destroyed, this, [this] {
        scene = new QGraphicsScene(this);
        m_map->populate();
        v_map->setScene(scene);
    });
    m_map->setSize(QSize(width, height));
    scene->deleteLater();
}

void MainWindow::createVisual()
{
    bool tmp;
    int height = m_map->getHeight();
    int width = m_map->getWidth();
    int count = 0;
    setMapRegen(true);

    if (scene == nullptr) return;

    QVector<VisualTile*> visualTiles;
    visualTiles.reserve(height * width);

    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            tmp = m_map->tileAt(column,row)->isWall();
            QColor color = tmp ? Qt::black : Qt::gray;
            VisualTile *item = new VisualTile();
            item->setColor(color);
            item->setCoords(column, row);
            item->setPos(QPointF((column)*50, (row)*50));

            QObject::connect(m_map->tileAt(column,row), &Tile::stateChanged, item, [item, this, column, row](){
                QColor color;
                switch (this->m_map->tileAt(column,row)->getState()) {
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
            QObject::connect(item, &VisualTile::mousePressed, m_map, &UserMap::resetStart);
            visualTiles.append(item);

            count++;
            if (count % 25 == 0) QCoreApplication::processEvents();
        }
    }
    for (VisualTile* item : visualTiles) {
        scene->addItem(static_cast<QGraphicsObject*>(item));
    }

    setMapRegen(false);
    v_map->zoomReset();
    v_map->tileMap()->centerOn(QPointF(width*25, height*25));
    v_map->zoomToFit();
    // v_map->tileMap()->geometry();

}

UserMap *MainWindow::getMap()
{
    return m_map;
}


bool MainWindow::isMapRegenON()
{
    return isMapRegen;
}

void MainWindow::setMapRegen(bool n_isMapRegen)
{
    isMapRegen = n_isMapRegen;
    searchButton->setEnabled(!isMapRegen);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("CraftWorld", "pathSearch");
    settings.setValue("geometry", saveGeometry());
    QWidget::closeEvent(event);
}

