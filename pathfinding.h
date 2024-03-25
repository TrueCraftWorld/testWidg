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
    bool breadthFirstSearch(QPoint start);
    bool bFS();


    void setGraph(UserMap* );
    void setStart(QPoint);

    UserMap * getGraph(void);
signals:
    void pathFound(bool);

private:
    UserMap * m_graph = nullptr;
    QPoint m_start;
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
