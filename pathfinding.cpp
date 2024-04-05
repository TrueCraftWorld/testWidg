
#include "usermap.h"
#include <QObject>
#include <QQueue>
#include <QSet>
#include <QPointer>


#include "pathfinding.h"


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
            if (foundTile != nullptr && knownTiles.contains(foundTile) == false) { //our neighbors are not walls and are new
                knownTiles.insert(foundTile);
                knownBorder.enqueue(foundTile);
                foundTile->setPrevious(beingCheked);
            }
        }
    }
    return true;
}
