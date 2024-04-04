
#include "usermap.h"
#include <QObject>
#include <QQueue>
#include <QSet>
#include <QSharedPointer>


#include "pathfinding.h"

PathSearch::PathSearch(QObject * parent)
{
    Q_UNUSED(parent);
}

void PathSearch::setGraph(UserMap * graph)
{
    m_graph = graph;
}

void PathSearch::setStart(QPoint start)
{
     m_start = start;
}

bool PathSearch::breadthFirstSearch(QPoint start)
{
    QQueue<Tile*> knownBorder; //only edge of known area
    QSet<Tile*> knownTiles; //all known tiles
    Tile* beingCheked = m_graph->tileAt(start.x() + start.y()*m_graph->getWidth());
    if (beingCheked == nullptr)  return false;

    knownBorder.enqueue(beingCheked);
    knownTiles.insert(beingCheked);
    beingCheked->setPrevious(beingCheked);

    while (!knownBorder.empty()) { //no need for exit on targetfound as we need to calculate whole map anyway
        beingCheked = knownBorder.dequeue();
        for (auto foundTile : beingCheked->neighbors) {
            if (foundTile != nullptr && knownTiles.contains(foundTile) == false) { //our neighbors are not walls and are new
                knownTiles.insert(foundTile);
                knownBorder.enqueue(foundTile);
                foundTile->setPrevious(beingCheked);
            }
        }
    }
    return true;
}

bool PathSearch::performSearch()
{
    bool result = breadthFirstSearch(m_start);
    emit pathFound(result);
    return result;
}
