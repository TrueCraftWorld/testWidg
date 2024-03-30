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


#endif // PATHFINDING_H
