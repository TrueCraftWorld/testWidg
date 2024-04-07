#ifndef PATHFINDING_H
#define PATHFINDING_H


#include "usermap.h"

class PathSearch
{
public:
    static bool breadthFirstSearch(QPoint start, UserMap * graph);
};



#endif // PATHFINDING_H
