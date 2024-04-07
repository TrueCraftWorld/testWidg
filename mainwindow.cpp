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
#include <QCoreApplication>
#include <QSettings>
#include <QLabel>
#include <QtConcurrent/QtConcurrent>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      scene(new QGraphicsScene(this)),
      m_map(new UserMap()),
      v_map(new MapView()),
      searchButton(new QPushButton(this)),
      validator(new QIntValidator(1,999)),
      widthEdit(new QLineEdit(this)),
      heightEdit(new QLineEdit(this)),
      regenLabel(new QLabel(this))
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

    regenLabel->setVisible(false);

    layoutV->addWidget(widthLabel);
    layoutV->addWidget(widthEdit);
    layoutV->addWidget(heightLabel);
    layoutV->addWidget(heightEdit);
    layoutV->addWidget(searchButton);
    layoutV->addStretch();
    layoutV->addWidget(regenLabel);
    layoutV->addStretch();

    layout->addWidget(v_map);
    layout->addLayout(layoutV);

    widthEdit->move(widthEdit->x(), v_map->y());

    setLayout(layout);
    setWindowTitle(tr("Path Test"));
    this->restoreGeometry(settings.value("geometry").toByteArray());

    QObject::connect(searchButton, &QPushButton::clicked,
                     this, [this](){
        setMapRegen(true);
    });
    QObject::connect(searchButton, &QPushButton::clicked, m_map, &UserMap::empty);
    QObject::connect(m_map, &UserMap::emptied,
                     this, [this]() {

        reGenerateMap(widthEdit->displayText().toInt(), heightEdit->displayText().toInt());
    });
    QObject::connect(m_map, &UserMap::mapReady, this, &MainWindow::createVisual);
}

/**
 * @brief deletes currentScene and start generatinon of new ones
 * 
 * @param width width of new map in Tiles
 * @param height height of new map in Tiles
 */
void MainWindow::reGenerateMap(int width, int height)
{
    QObject::connect(scene, &QObject::destroyed, this, [this] {
        scene = new QGraphicsScene(this);
        QCoreApplication::processEvents();
        m_map->populate();// this will provide backend map
    });
    QCoreApplication::processEvents();//to show map state label
    m_map->setSize(QSize(width, height));
    scene->deleteLater();
}

/**
 * @brief creates and sets up neede amount of visualTiles. placing them into sceen
 * 
 */
void MainWindow::createVisual()
{
    bool tmp;
    int height = m_map->getHeight();
    int width = m_map->getWidth();
    int count = 0;
    regenLabel->setText(QString("Rendering MAP"));

    if (scene == nullptr) return;

    scene->setItemIndexMethod(QGraphicsScene::NoIndex); //we need no index but speed of adding item is important
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            tmp = m_map->tileAt(column,row)->isWall();
            QColor color = tmp ? Qt::black : Qt::gray;
            QPointer<VisualTile> item = new VisualTile();
            item->setColor(color);
            item->setCoords(column, row);
            item->setPos(QPointF((column)*tileSize, (row)*tileSize));
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
            scene->addItem(static_cast<QGraphicsObject*>(item));
            count++;
            if (count % 15000 == 0) QCoreApplication::processEvents(); //chip example were running good
        }
    }
    setMapRegen(false);
    v_map->setScene(scene);
    v_map->tileMap()->centerOn(QPointF((width*tileSize)/2, (height*tileSize)/2));
    v_map->zoomToFit();
}

UserMap *MainWindow::getMap()
{
    return m_map;
}

/**
 * @brief performs needed GUI restriction if n_isMapRegen set
 * 
 * @param n_isMapRegen show if map is currently being regenerated
 */
void MainWindow::setMapRegen(bool n_isMapRegen)
{
    isMapRegen = n_isMapRegen;
    searchButton->setEnabled(!isMapRegen);
    widthEdit->setReadOnly(isMapRegen);
    heightEdit->setReadOnly(isMapRegen);
    if (isMapRegen) regenLabel->setText(QString("Generating MAP"));
    regenLabel->setVisible(isMapRegen);
    regenLabel->update();
}

/**
 * @brief save window geometry and passes clise event on
 * 
 * @param event 
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("CraftWorld", "pathSearch");
    settings.setValue("geometry", saveGeometry());
    QWidget::closeEvent(event);
}

