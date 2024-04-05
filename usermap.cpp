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
    if (m_state == state) return;
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


void UserMap::setSize(QSize size)
{
    m_size = size;
}

void UserMap::populate()
{
    // QVector<QSharedPointer<Tile>>
    if (!m_size.isValid()) return;
    QThreadPool pool;
    int chunk = 5000;
    int totelTiles = m_size.height()*m_size.width();
    int runs = 0;
    int tilesToDo;
    m_tiles.resize(totelTiles);
    QFutureSynchronizer<void> synchronizer;
    pool.setMaxThreadCount(6);
    while (totelTiles) {

        if (totelTiles > chunk) {
            tilesToDo = chunk;
            totelTiles -= chunk;
        } else {
            tilesToDo = totelTiles;
            totelTiles = 0;
        }
        int index = runs*chunk;
        runs++;
        synchronizer.addFuture( QtConcurrent::run(&pool,[this, tilesToDo, index](){
            if (!this->m_size.isValid()) return;
            int wall = false;
            // gen_tiles.reserve(chunk);
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
        }));
    }
    synchronizer.waitForFinished();
    connectMap();
    emit mapReady();
}

void UserMap::search(QPoint point)
{
    m_start = point;
    QFuture<bool> future = QtConcurrent::run(PathSearch::breadthFirstSearch, point, this);
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

QPointer<Tile> UserMap::tileAt(int index)
{
    if (m_tiles.size() <= index) return nullptr;
    return m_tiles.at(index).get();
}

QPointer<Tile> UserMap::tileAt(int x, int y)
{
    return tileAt(x + (y * m_size.width()) );
}

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



QVector<QSharedPointer<Tile> > MapGenerator::returnMap()
{
    return gen_tiles;
}

void MapGenerator::setSize(QSize size)
{
    if (size.isValid()) gen_size = size;
}


/**
 * @brief MapGenerator::generateMap more maze-like version of random map
 * @details in all odd row and colums places a wal with a certain chance
 *          all cells in even row OR even column are not walls
 */
void MapGenerator::generateMap()
{
    if (!gen_size.isValid()) return;

    int wall = false;
    gen_tiles.reserve(gen_size.width() * gen_size.height());
    for (int  row = 0; row < gen_size.height(); ++row) {
        for (int column = 0; column < gen_size.width(); ++column) {
            wall = false;
            if (row % 2) {
                if (QRandomGenerator::global()->bounded(100) > 55){
                    wall = true;
                }
            }
            if (column % 2) {
                if (QRandomGenerator::global()->bounded(100) > 45){
                    wall = true;
                }
            }
            QSharedPointer<Tile> tile_ptr (new Tile());

            tile_ptr->setCoords(QPoint(column,row));
            tile_ptr->setWall(wall);
            tile_ptr->neighbors << nullptr << nullptr << nullptr << nullptr;
            gen_tiles.append(tile_ptr);
        }
    }
    emit mapCreated();
}

