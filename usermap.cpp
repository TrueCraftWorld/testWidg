#include "usermap.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <QPoint>
#include <QSize>

Tile::Tile(int x, int y, bool wall)
    : m_coords{x, y}, m_wall{wall}
{}

QPoint Tile::getCoords()
{
    return m_coords;
}

void Tile::setWall(bool wall)
{
    m_wall = wall;
}

bool Tile::isWall()
{
    return m_wall;
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
            if (std::rand() > RAND_MAX/3){
                wall = false;
            } else {
                wall = true;
            }
            if (i == 0 && j == 0)  wall = false;
            std::shared_ptr <Tile> tile_ptr (new Tile(j, i, wall));
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

void UserMap::connectMap() {

    for (unsigned j = 0; j < m_size.height(); ++j) {
        for (unsigned i = 0; i < m_size.width(); ++i) {
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


