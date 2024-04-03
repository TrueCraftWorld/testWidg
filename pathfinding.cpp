
#include "usermap.h"
#include <QObject>
#include <queue>
#include <unordered_set>


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
    std::queue<Tile*> knownBorder; //only edge of known area
    std::unordered_set<Tile*> knownTiles; //all known tiles

    Tile* beingCheked = m_graph->tileAt(start.x() + start.y()*m_graph->getWidth());
    if (beingCheked == nullptr)  return false;

    knownBorder.push(beingCheked);
    knownTiles.insert(beingCheked);
    beingCheked->setPrevious(beingCheked);

    while (!knownBorder.empty()) { //no need for exit on targetfound as we need to calculate whole map anyway
        beingCheked = knownBorder.front();
        knownBorder.pop(); // at this point our location is not ON border but inside
        // curNumber = beingCheked->getCoords().x() + beingCheked->getCoords().y() * m_graph->getWidth();
        for (Tile* foundTile : beingCheked->neighbors) {
            if (foundTile != nullptr &&
                knownTiles.find(foundTile) == knownTiles.end() ) { //our neighbors are not walls
// actions only for actually new Tiles
                knownTiles.insert(foundTile);
                knownBorder.push(foundTile);
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
