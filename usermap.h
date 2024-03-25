#ifndef USERMAP_H
#define USERMAP_H

#include <array>
#include <memory>
#include <QPoint>
#include <QObject>
#include <QSize>
#include <QVector>
#include <QSharedPointer>
#include <iostream>

// enum class States{EMPTY, WALL, START, STOP, PATH};

enum {UP, RIGHT, DOWN, LEFT};

class Tile : public QObject
{
    Q_OBJECT
public:
    enum class States {
        EMPTY,
        WALL,
        START,
        STOP,
        PATH
    }; Q_ENUM(States)
    explicit Tile(QObject *_parent = nullptr) {}
    // Tile(int x, int y, bool wall =false);

    QPoint getCoords();
    void setCoords(const QPoint&);

    void setWall(bool);
    bool isWall();
    States getState();
    void setState(States);

    std::array<Tile*, 4> neighbors = {nullptr,nullptr,nullptr,nullptr};
    // std::array<Tile*, 8> neighbors;
signals:
    void stateChanged();

private:
    QPoint m_coords;


public:
    Tile* getPrevious();
    void setPrevious(Tile* prev);
private:
    Tile* m_previous = nullptr; //inhereting simple tile to add backtracking
    States m_state;
    // std::array<MOVE, 4> m_directions = {0,1},{1,0},{0,-1},{-1,0};
    // std::array<MOVE, 8> Directions = {{0,1},{1,0},{0,-1},{-1,0},
    //                                   {-1,1},{1,1},{1,-1},{-1,-1}};

};


class UserMap : public QObject
{
    Q_OBJECT
public:
    explicit UserMap(QObject *_parent = nullptr) {}
    // UserMap(QWidget *parent = nullptr, int width=10, int height=10, QPoint goal = QPoint(0,0));
    // ~UserMap();

    void setSize(QSize size);
    void create();

    int getWidth();
    int getHeight();
    void addTile(QSharedPointer<Tile>);
    Tile* tileAt(int index);
    Tile* tileAt(int x, int y);
    void highlightPath(QPoint, bool);
    void setGoal(QPoint);
    void unsetGoal(QPoint);
    QPoint getGoal();

private:
    QVector<QSharedPointer<Tile>> m_tiles;
    QSize m_size;
    QPoint m_goal;
    void connectMap();
};




#endif // USERMAP_H
