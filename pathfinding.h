#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "usermap.h"

class PathSearch
{
public:
    PathSearch();
    static bool breadthFirstSearch(UserMap& graph, Point start, Point goal = {0,0});
    static void printPath(UserMap& graph, Point goal = {0,0});
};

// class TrackTile : public Tile
// {
// public:
//     TrackTile* getPrevious();
//     setPrevious(TrackTile*);
// private:
//     TrackTile* previous = nullptr; //inhereting simple tile to add backtracking
// };

#endif // PATHFINDING_H
