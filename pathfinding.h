#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include "usermap.h"

class PathSearch : public QObject
{
    Q_OBJECT
public:
    explicit PathSearch(QObject * parent = nullptr);
    // PathSearch(UserMap *, QObject * parent = nullptr);
    bool breadthFirstSearch(QPoint start, QPoint goal = QPoint(0,0));
    bool bFS();
    static void printPath(UserMap& ,QPoint);
    void setGraph(UserMap* );
    void setStart(QPoint);
    void setGoal(QPoint);
    UserMap * getGraph(void);
signals:
    void pathFound(bool);

private:
    UserMap * m_graph = nullptr;
    QPoint m_start;
    QPoint m_goal;
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
