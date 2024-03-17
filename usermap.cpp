#include "usermap.h"
#include <cstdlib>
#include <ctime>

Tile::Tile(unsigned x, unsigned y, bool wall)
    // m_coords(Point(x, y)), m_wall(wall)
{
    m_coords.x = x;
    m_coords.y = y;
    m_wall = wall;
}

Point Tile::getCoords()
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

UserMap::UserMap(unsigned width, unsigned height)
    // m_size(MapSize(width, height))
{
    m_size.width = width;
    m_size.height = height;
    std::srand(42);
    bool wall = false;
    for (unsigned i = 0; i < m_size.height; ++i) {
        for (unsigned j = 0; j < m_size.width; ++j) {
            if (j%2 == 0) {
                wall = false;
            } else if (std::rand() > RAND_MAX/3){
                wall = true;
            } else {
                wall = false;
            }
            m_tiles.push_back(new Tile(j, i, wall));
        }
    }
    connectMap();
}

unsigned int UserMap::getWidth()
{
    return m_size.width;
}

unsigned int UserMap::getHeight()
{
    return m_size.height;
}

void UserMap::connectMap() {

    for (unsigned j = 0; j < m_size.height; ++j) {
        for (unsigned i = 0; i < m_size.width; ++i) {
            m_tiles.at(i + m_size.width*j)->neighbors.push_back(nullptr);
            m_tiles.at(i + m_size.width*j)->neighbors.push_back(nullptr);
            m_tiles.at(i + m_size.width*j)->neighbors.push_back(nullptr);
            m_tiles.at(i + m_size.width*j)->neighbors.push_back(nullptr);


            if (i > 0) {
                if (!m_tiles.at(i + m_size.width*j - 1)->isWall())
                     m_tiles.at(i + m_size.width*j)->neighbors.at(LEFT) = (m_tiles.at(i + m_size.width*j - 1));
            }

            if (i < (m_size.width - 1)) {
                if (!m_tiles.at(i + m_size.width*j + 1)->isWall())
                     m_tiles.at(i + m_size.width*j)->neighbors.at(RIGHT) = (m_tiles.at(i + m_size.width*j + 1));
            }

            if (j > 0) {
                if (!m_tiles.at(i + m_size.width*(j - 1))->isWall())
                     m_tiles.at(i + m_size.width*j)->neighbors.at(UP) = (m_tiles.at(i + m_size.width*(j - 1)));
            }
            if (j < (m_size.height - 1)) {
                if (!m_tiles.at(i + m_size.width*(j + 1))->isWall())
                     m_tiles.at(i + m_size.width*j)->neighbors.at(DOWN) = (m_tiles.at(i + m_size.width*(j + 1)));
            }
        }
    }
}


bool Point::operator==(const Point &right)
{
    return ((this->x == right.x) && (this->y == right.y));
}
