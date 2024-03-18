#include "pathfinding.h"
#include "usermap.h"
#include <queue>
#include <unordered_set>
#include <iostream>

PathSearch::PathSearch()
{

}

void PathSearch::printPath(UserMap& graph, Point goal)
{
    Tile* backTrackStart = graph.m_tiles.at(goal.x + goal.y*graph.getWidth()).get();
    unsigned curNumber;
    while (backTrackStart != (backTrackStart->getPrevious())) {
            curNumber = backTrackStart->getCoords().x + backTrackStart->getCoords().y * graph.getWidth();
            std::cout << "N=" << curNumber << "<--";
            backTrackStart = (backTrackStart->getPrevious());
    }
    curNumber = backTrackStart->getCoords().x + backTrackStart->getCoords().y * graph.getWidth();
    std::cout << "N=" << curNumber;
    std::cout << std::endl;
}

bool PathSearch::breadthFirstSearch(UserMap& graph, Point start, Point goal)
{
    std::queue<Tile*> knownBorder; //only edge of known area
    std::unordered_set<Tile*> knownTiles; //all known tiles
    bool found = false;
    Tile* beingCheked = graph.m_tiles.at(start.x + start.y*graph.getWidth()).get();
    knownBorder.push(beingCheked);
    knownTiles.insert(beingCheked);
    unsigned curNumber = 0;

    if (beingCheked != nullptr) beingCheked->setPrevious(beingCheked);

    while (!knownBorder.empty() /*&& (!found)*/) {
        beingCheked = knownBorder.front();
        knownBorder.pop(); // at this point our location in not ON border but inside
        curNumber = beingCheked->getCoords().x + beingCheked->getCoords().y * graph.getWidth();
        std::cout << "  Visiting N=" << curNumber << std::endl;
        for (Tile* foundling : beingCheked->neighbors) {
            if (foundling != nullptr) { //our neighbors are not walls
                if (knownTiles.find(foundling) == knownTiles.end()) { // actions only for actually new Tiles
                    knownTiles.insert(foundling);
                    knownBorder.push(foundling);
                    foundling->setPrevious(beingCheked);
                    if (foundling->getCoords() == goal) {
                        std::cout << "  found goal" << std::endl;
                        found = true;
                        // break;
                    }
                }
            }
        }
    }
    if (found) return true;
    else return false;
}
