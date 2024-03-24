#include "usermap.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <QPoint>
#include <QSize>
#include <QSharedPointer>

// Tile::Tile(int x, int y, bool wall)
//     : m_coords{x, y}, m_wall{wall}
// {}

QPoint Tile::getCoords()
{
    return m_coords;
}

void Tile::setCoords(const QPoint &coords)
{
    m_coords = coords;
}

void Tile::setWall(bool wall)
{
    if (wall) m_state = States::WALL;
}

bool Tile::isWall()
{
    return (m_state == States::WALL);
}

Tile::States Tile::getState()
{
    return m_state;
}

void Tile::setState(States state)
{
    if (m_state == state) return;
    m_state = state;
    emit stateChanged();
}

Tile* Tile::getPrevious()
{
    return m_previous;
}

void Tile::setPrevious(Tile* prev)
{
    m_previous = prev;
}

UserMap::UserMap(int width, int height)
   : m_size(width, height)
{
    std::srand(std::time(0));
    bool wall = false;
    for (int i = 0; i < m_size.height(); ++i) {
        for (int j = 0; j < m_size.width(); ++j) {
            if (std::rand() > RAND_MAX/4){
                wall = false;
            } else {
                wall = true;
            }
            if (i == 0 && j == 0)  wall = false;
            QSharedPointer<Tile> tile_ptr (new Tile());
            tile_ptr->setCoords(QPoint(j,i));
            tile_ptr->setWall(wall);

            m_tiles.push_back(tile_ptr);

        }
    }
    connectMap();
}


int UserMap::getWidth()
{
    return m_size.width();
}

int UserMap::getHeight()
{
    return m_size.height();
}

Tile* UserMap::tileAt(int index)
{
    return m_tiles.at(index).get();
}

Tile* UserMap::tileAt(int x, int y)
{
    return tileAt(x + (y * m_size.width()) );
}

void UserMap::connectMap() {

    for (int j = 0; j < m_size.height(); ++j) {
        for (int i = 0; i < m_size.width(); ++i) {
            // m_tiles.at(i + m_size.width()*j)->neighbors.push_back(nullptr);
            // m_tiles.at(i + m_size.width()*j)->neighbors.push_back(nullptr);
            // m_tiles.at(i + m_size.width()*j)->neighbors.push_back(nullptr);
            // m_tiles.at(i + m_size.width()*j)->neighbors.push_back(nullptr);

            Tile* tmp = m_tiles.at(i + m_size.width()*j).get();
            Tile* mbNeighbor = nullptr;
            if (i > 0) {
                mbNeighbor = m_tiles.at(i + m_size.width()*j - 1).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(LEFT) = mbNeighbor;
            }

            if (i < (m_size.width() - 1)) {
                mbNeighbor = m_tiles.at(i + m_size.width()*j + 1).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(RIGHT) = mbNeighbor;
            }

            if (j > 0) {
                mbNeighbor = m_tiles.at(i + m_size.width()*(j - 1)).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(UP) = mbNeighbor;
            }
            if (j < (m_size.height() - 1)) {
                mbNeighbor = m_tiles.at(i + m_size.width()*(j + 1)).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(DOWN) = mbNeighbor;
            }
        }
    }
}


