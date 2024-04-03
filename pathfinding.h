#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include "usermap.h"

class PathSearch : public QObject
{
    Q_OBJECT
public:
    explicit PathSearch(QObject * parent = nullptr);
    bool breadthFirstSearch(QPoint start);
    bool performSearch();

    void setGraph(UserMap*);
    UserMap * getGraph(void);

    void setStart(QPoint);

signals:
    void pathFound(bool);

private:
    UserMap * m_graph = nullptr;
    QPoint m_start;
};


#endif // PATHFINDING_H
