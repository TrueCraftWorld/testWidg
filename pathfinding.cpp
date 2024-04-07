
#include "usermap.h"
#include <QObject>
#include <QQueue>
#include <QSet>
#include <QPointer>


#include "pathfinding.h"

/**
 * @brief explores given graph from start point. 
 *          sets up backtrack pointers
 * 
 * @param start QPoint determines from where search is performed
 * @param graph graph of Tiles
 * @return true search completed
 * @return false invalid graph passed (search still completed on valid part)
 */
bool PathSearch::breadthFirstSearch(QPoint start, UserMap * graph)
{
    QQueue<Tile *> knownBorder; //only edge of known area
    QSet<Tile *> knownTiles; //all known tiles
    QPointer<Tile> beingCheked = graph->tileAt(start.x() + start.y()*graph->getWidth());
    if (beingCheked == nullptr)  return false;

    knownBorder.enqueue(beingCheked);
    knownTiles.insert(beingCheked);
    beingCheked->setPrevious(beingCheked);

    while (!knownBorder.empty()) { //no need for exit on targetfound as we need to calculate whole map anyway
        beingCheked = knownBorder.dequeue();
        for (auto foundTile : beingCheked->neighbors) {
            if (foundTile == nullptr) continue;
            if (knownTiles.contains(foundTile) == false) { //our neighbors are not walls and are new
                knownTiles.insert(foundTile);
                knownBorder.enqueue(foundTile);
                foundTile->setPrevious(beingCheked);
            }
        }
    }
    return true;
}
