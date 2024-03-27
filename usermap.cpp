#include "pathfinding.h"

#include "usermap.h"
#include <QRandomGenerator>
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

    int wall = false;
    for (int  row = 0; row < m_size.height(); ++row) {
        for (int column = 0; column < m_size.width(); ++column) {
            if (QRandomGenerator::global()->bounded(100) > 25){
                wall = false;
            } else {
                wall = true;
            }
            if (row == 0 && column == 0)  wall = false;
            QSharedPointer<Tile> tile_ptr (new Tile());

            tile_ptr->setCoords(QPoint(column,row));
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
    if (tileAt(point.x(), point.y())->isWall()) return;
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
    backTrackStart = (backTrackStart->getPrevious());
    while (backTrackStart != (backTrackStart->getPrevious())) {
        backTrackStart->setState(hide ? Tile::States::EMPTY : Tile::States::PATH);
        backTrackStart = (backTrackStart->getPrevious());
    }
    backTrackStart->setState(hide ? Tile::States::EMPTY : Tile::States::START);
    std::cout << "Success" << std::endl;
}

void UserMap::unsetGoal(QPoint old_goal)
{
    // if (tileAt(old_goal.x(),old_goal.y())->isWall()) return;
    // tileAt(old_goal.x(),old_goal.y())->setState(Tile::States::EMPTY);
    // highlightPath(old_goal, true);
}

void UserMap::connectMap() {

    for (int row = 0; row < m_size.height(); ++row) {
        for (int column = 0; column < m_size.width(); ++column) {

            Tile* tmp = m_tiles.at(column + m_size.width()*row).get();
            Tile* mbNeighbor = nullptr;
            if (column > 0) {
                mbNeighbor = m_tiles.at(column + m_size.width()*row - 1).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(LEFT) = mbNeighbor;
            }

            if (column < (m_size.width() - 1)) {
                mbNeighbor = m_tiles.at(column + m_size.width()*row + 1).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(RIGHT) = mbNeighbor;
            }

            if (row > 0) {
                mbNeighbor = m_tiles.at(column + m_size.width()*(row - 1)).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(UP) = mbNeighbor;
            }
            if (row < (m_size.height() - 1)) {
                mbNeighbor = m_tiles.at(column + m_size.width()*(row + 1)).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.at(DOWN) = mbNeighbor;
            }
        }
    }
}

void UserMap::setGoal(QPoint goal)
{
    if (tileAt(goal.x(),goal.y())->isWall()) return;

    tileAt(m_goal.x(),m_goal.y())->setState(Tile::States::EMPTY);
    highlightPath(m_goal, true);

    tileAt(goal.x(),goal.y())->setState(Tile::States::STOP);

    highlightPath(goal, false);
    m_goal = goal;
}

QPoint UserMap::getGoal()
{
    return m_goal;
}

void UserMap::empty()
{
    m_tiles.clear();
    m_start = QPoint(0,0);
    m_goal = QPoint(0,0);
    m_size = QSize(0,0);
    emit emptied();
}


