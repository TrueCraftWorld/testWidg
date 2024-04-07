#include "pathfinding.h"

#include "usermap.h"
#include <QRandomGenerator>
#include <QPoint>
#include <QSize>
#include <QSharedPointer>
#include <QtConcurrent/QtConcurrent>
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
    m_state = state;
    emit stateChanged();
}



QPointer<Tile> Tile::getPrevious()
{
    return m_previous;
}

void Tile::setPrevious(QPointer<Tile> prev)
{
    m_previous = prev;
}


UserMap::UserMap(QObject *parent)
{
    Q_UNUSED(parent)
    QObject::connect(this, &UserMap::mapPartReady, this, &UserMap::checkMapCreation);
}

void UserMap::setSize(QSize size)
{
    m_size = size;
}

/**
 * @brief create (width*height) Tiles in multiple threads 
 * @details splits creation into batches by 5000 pieces
 *          Tiles are being quazi randomly set as Tile::States::WALL
 */
void UserMap::populate()
{
    if (!m_size.isValid()) return;

    int totelTiles = m_size.height()*m_size.width();
    int tilesToDo;
    m_tiles.resize(totelTiles); //
    int numThreads = QThread::idealThreadCount(); // noexept always returns at least 1
    m_mapParts = numThreads;
    int chunk = totelTiles / numThreads;
    int index = 0;
    QFutureSynchronizer<void> synchronizer;
    for (int i = 0; i < numThreads; ++i) {

        tilesToDo = chunk;
        if (i == 0 && numThreads != 1) tilesToDo += totelTiles % numThreads; //округлённые при делении тайлы в первый поток
        synchronizer.addFuture( QtConcurrent::run([this, tilesToDo, index](){
            if (!this->m_size.isValid()) return;
            int wall = false;
            for (int i = index; i < index + tilesToDo; ++i) {
                wall = false;
                if (i/this->m_size.width() % 2) {
                    if (QRandomGenerator::global()->bounded(100) > 55){
                        wall = true;
                    }
                }
                if (i%this->m_size.width() % 2) {
                    if (QRandomGenerator::global()->bounded(100) > 45){
                        wall = true;
                    }
                }
                QSharedPointer<Tile> tile_ptr (new Tile());
                tile_ptr->setCoords(QPoint(i%this->m_size.width(),i/this->m_size.width()));
                tile_ptr->setWall(wall);
                tile_ptr->neighbors << nullptr << nullptr << nullptr << nullptr;
                this->m_tiles[i] = tile_ptr;
            }
            emit this->mapPartReady();
        }));
        index += tilesToDo;
    }
    // setThreadWaiting(numThreads);
    // synchronizer.waitForFinished();
    // connectMap();
    // emit mapReady();
}

/**
 * @brief launches search bFS search in another thread
 * 
 * @param point 
 */
void UserMap::search(QPoint point)
{
    m_start = point;
    QFuture<bool> future = QtConcurrent::run(PathSearch::breadthFirstSearch, point, this);
}

/**
 * @brief changes start point to given point
 * @details when given correct input hide current path in scene, calls new search
 * @param point new start point
 */
void UserMap::resetStart(QPoint point)
{
    
    if (m_start == point) return;
    if (tileAt(point.x(), point.y())->isWall()) return;
    if (point.x() >= m_size.width()  ||
        point.x() < 0                ||
        point.y() >= m_size.height() ||
        point.y() < 0 ) return;
    highlightPath(point, true);
    clearPath();
    search(point);
}

/**
 * @brief clears highlithed path
 * 
 */
void UserMap::clearPath()
{
    for (auto tmp : m_tiles)
    {
        tmp.get()->setPrevious(nullptr);
        if (tmp.get()->getState() != Tile::States::WALL)
            tmp.get()->setState(Tile::States::EMPTY);
    }
}

void UserMap::checkMapCreation()
{
    if (m_mapParts == 0) return;
    m_mapParts--;
    if (m_mapParts != 0) return;
    QFuture<void> someFuture = QtConcurrent::run(this, &UserMap::connectMap);
}

int UserMap::getWidth()
{
    return m_size.width();
}

int UserMap::getHeight()
{
    return m_size.height();
}

QPointer<Tile> UserMap::tileAt(int index)
{
    if (m_tiles.size() <= index) return nullptr;
    return m_tiles.at(index).get();
}

QPointer<Tile> UserMap::tileAt(int x, int y)
{
    return tileAt(x + (y * m_size.width()) );
}

/**
 * @brief hide/highlights path from start to goal if exists
 * 
 * @param goal point to hide/higlith path to
 * @param hide selects hide/hilight path
 * 
 */
void UserMap::highlightPath(QPoint goal, bool hide)
{
    QPointer<Tile> backTrackStart = tileAt(goal.x() + goal.y()*m_size.width());
    if (backTrackStart->getPrevious() == nullptr) { //if path exists it is known, otherwise we know there is no path
        return;
    }
    backTrackStart = backTrackStart->getPrevious();
    while (backTrackStart != (backTrackStart->getPrevious())) {
        backTrackStart->setState(hide ? Tile::States::EMPTY : Tile::States::PATH);
        backTrackStart = backTrackStart->getPrevious();
    }
    backTrackStart->setState(hide ? Tile::States::EMPTY : Tile::States::START);
}

void UserMap::unsetGoal(QPoint old_goal)
{
    Q_UNUSED(old_goal)
}

/**
 * @brief iterates through map in set neighbours field
 * 
 */
void UserMap::connectMap() {

    for (int row = 0; row < m_size.height(); ++row) {
        for (int column = 0; column < m_size.width(); ++column) {

            QPointer<Tile> tmp = m_tiles.at(column + m_size.width()*row).get();
            QPointer<Tile> mbNeighbor = nullptr;
            if (column > 0) {
                mbNeighbor = m_tiles.at(column + m_size.width()*row - 1).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.replace(LEFT, mbNeighbor);
            }

            if (column < (m_size.width() - 1)) {
                mbNeighbor = m_tiles.at(column + m_size.width()*row + 1).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.replace(RIGHT, mbNeighbor);;
            }

            if (row > 0) {
                mbNeighbor = m_tiles.at(column + m_size.width()*(row - 1)).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.replace(UP, mbNeighbor);
            }
            if (row < (m_size.height() - 1)) {
                mbNeighbor = m_tiles.at(column + m_size.width()*(row + 1)).get();
                if (!mbNeighbor->isWall())
                    tmp->neighbors.replace(DOWN, mbNeighbor);
            }
        }
    }
    emit mapReady();
}

/**
 * @brief processes selection of new goal
 * 
 * @param goal new goal
 */
void UserMap::setGoal(QPoint goal)
{

    if (m_goal == goal) return;
    if (tileAt(goal.x(), goal.y())->isWall()) return;
    if (goal.x() >= m_size.width()  ||
        goal.x() < 0                ||
        goal.y() >= m_size.height() ||
        goal.y() < 0 ) return;

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


