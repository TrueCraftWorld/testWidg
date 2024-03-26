#include "pathfinding.h"

#include "usermap.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <QPoint>
#include <QSize>
#include <QSharedPointer>
#include <QThread>

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
    if (wall) m_state = Tile::States::WALL;
}

bool Tile::isWall()
{
    return (m_state == Tile::States::WALL);
}

Tile::States Tile::getState()
{
    return m_state;
}

void Tile::setState(Tile::States state)
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


void UserMap::setSize(QSize size)
{
    m_size = size;
}

void UserMap::create()
{
    if (!m_size.isValid()) return;
    // m_tiles.resize(m_size.width()*m_size.height());
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
            QSharedPointer<Tile> tile_ptr (new Tile());

            tile_ptr->setCoords(QPoint(j,i));
            tile_ptr->setWall(wall);

            m_tiles.push_back(tile_ptr);

        }
    }
    connectMap();
}

void UserMap::search(QPoint point)
{
    m_start = point;
    PathSearch* pS = new PathSearch();
    pS->setGraph(this);
    pS->setStart(point);

    QThread * thread = new QThread();
    pS->moveToThread(thread);
    QObject::connect(thread, &QThread::started, pS, &PathSearch::bFS);
    QObject::connect(thread, &QThread::finished, pS, &QObject::deleteLater);
    QObject::connect(pS, &PathSearch::pathFound, thread, &QThread::quit);
    thread->start();
}

void UserMap::resetStart(QPoint point)
{
    highlightPath(point, true);
    clearPath();
    search(point);
}

void UserMap::clearPath()
{
    for (auto tmp : m_tiles)
    {
        tmp.get()->setPrevious(nullptr);
        if (tmp.get()->getState() != Tile::States::WALL)
            tmp.get()->setState(Tile::States::EMPTY);
    }
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
    if (m_tiles.size() <= index) return nullptr;
    return m_tiles.at(index).get();
}

Tile* UserMap::tileAt(int x, int y)
{
    return tileAt(x + (y * m_size.width()) );
}

void UserMap::highlightPath(QPoint goal, bool hide)
{
    Tile* backTrackStart = tileAt(goal.x() + goal.y()*m_size.width());
    if (backTrackStart->getPrevious() == nullptr) { //if path exists it is known, otherwise we know there is no path
        std::cout << "fail" << std::endl;
        return;
    }
    while (backTrackStart != (backTrackStart->getPrevious())) {
        backTrackStart = (backTrackStart->getPrevious());

        backTrackStart->setState(hide ? Tile::States::EMPTY : Tile::States::PATH);

    }
    std::cout << "Success" << std::endl;
}

void UserMap::unsetGoal(QPoint old_goal)
{
    // highlightPath(old_goal, true);
    // m_goal =
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

void UserMap::setGoal(QPoint goal)
{
    if(m_tiles.at(goal.x() + m_size.width()*goal.y()).get()->isWall()) return;
    highlightPath(m_goal, true);
    highlightPath(goal, false);
    m_goal = goal;
}

QPoint UserMap::getGoal()
{
    return m_goal;
}


