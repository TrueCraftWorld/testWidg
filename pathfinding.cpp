
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
    bool found = false;
    Tile* beingCheked = m_graph->tileAt(start.x() + start.y()*m_graph->getWidth());
    knownBorder.push(beingCheked);
    knownTiles.insert(beingCheked);
    unsigned curNumber = 0;

    if (beingCheked != nullptr) beingCheked->setPrevious(beingCheked);

    while (!knownBorder.empty() /*&& (!found)*/) {
        beingCheked = knownBorder.front();
        knownBorder.pop(); // at this point our location in not ON border but inside
        curNumber = beingCheked->getCoords().x() + beingCheked->getCoords().y() * m_graph->getWidth();
        // std::cout << "  Visiting N=" << curNumber << std::endl;
        for (Tile* foundling : beingCheked->neighbors) {
            if (foundling != nullptr) { //our neighbors are not walls
                if (knownTiles.find(foundling) == knownTiles.end()) { // actions only for actually new Tiles
                    knownTiles.insert(foundling);
                    knownBorder.push(foundling);
                    foundling->setPrevious(beingCheked);
                }
            }
        }
    }
    return true;
}

bool PathSearch::bFS()
{
    bool result = breadthFirstSearch(m_start);
    emit pathFound(result);
    return result;
}
