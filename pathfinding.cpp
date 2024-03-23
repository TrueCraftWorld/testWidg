#include "pathfinding.h"
#include "usermap.h"
#include <QObject>
#include <queue>
#include <unordered_set>
#include <iostream>

PathSearch::PathSearch(QObject * parent)
{
    // Q_UNUSED parent;
}

void PathSearch::setGraph(UserMap * graph)
{
    m_graph = graph;
}

void PathSearch::printPath(UserMap& graph, QPoint goal)
{
    Tile* backTrackStart = graph.m_tiles.at(goal.x() + goal.y()*graph.getWidth()).get();
    unsigned curNumber;
    while (backTrackStart != (backTrackStart->getPrevious())) {
            curNumber = backTrackStart->getCoords().x() + backTrackStart->getCoords().y() * graph.getWidth();
            std::cout << "N=" << curNumber << "<--";
            backTrackStart = (backTrackStart->getPrevious());
            if (backTrackStart == nullptr) return;
    }
    curNumber = backTrackStart->getCoords().x() + backTrackStart->getCoords().y() * graph.getWidth();
    std::cout << "N=" << curNumber;
    std::cout << std::endl;
}

void PathSearch::setStart(QPoint start)
{
     m_start = start;
}

void PathSearch::setGoal(QPoint goal)
{
    m_goal = goal;
}

bool PathSearch::breadthFirstSearch(QPoint start, QPoint goal)
{
    std::queue<Tile*> knownBorder; //only edge of known area
    std::unordered_set<Tile*> knownTiles; //all known tiles
    bool found = false;
    Tile* beingCheked = m_graph->m_tiles.at(start.x() + start.y()*m_graph->getWidth()).get();
    knownBorder.push(beingCheked);
    knownTiles.insert(beingCheked);
    unsigned curNumber = 0;

    if (beingCheked != nullptr) beingCheked->setPrevious(beingCheked);

    while (!knownBorder.empty() /*&& (!found)*/) {
        beingCheked = knownBorder.front();
        knownBorder.pop(); // at this point our location in not ON border but inside
        curNumber = beingCheked->getCoords().x() + beingCheked->getCoords().y() * m_graph->getWidth();
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

bool PathSearch::bFS()
{
    bool result = breadthFirstSearch(m_start, m_goal);
    emit pathFound(result);
    return result;
}
