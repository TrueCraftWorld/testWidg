#ifndef USERMAP_H
#define USERMAP_H

#include <array>
#include <QPoint>
#include <QObject>
#include <QSize>
#include <QVector>
#include <QSharedPointer>


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

    QPoint getCoords();
    void setCoords(const QPoint&);

    void setWall(bool);
    bool isWall();

    States getState();
    void setState(States);

    Tile * getPrevious();
    void setPrevious(Tile * prev);

    std::array<Tile *, 4> neighbors = {nullptr,nullptr,nullptr,nullptr};

signals:
    void stateChanged();

private:
    QPoint m_coords;
    Tile * m_previous = nullptr; //inhereting simple tile to add backtracking
    States m_state;
};



class UserMap : public QObject
{
    Q_OBJECT
public:
    void setSize(QSize size);
    void populate();
    void search(QPoint);
    void resetStart(QPoint);
    int getWidth();
    int getHeight();
    Tile * tileAt(int index);
    Tile * tileAt(int x, int y);
    void highlightPath(QPoint, bool);
    void setGoal(QPoint);
    void unsetGoal(QPoint);
    QPoint getGoal();

    void empty();

signals:
    void emptied();
    void mapReady();

private:
    QVector<QSharedPointer<Tile>> m_tiles;
    QSize m_size;
    QPoint m_goal;
    QPoint m_start;
    void connectMap();
    void clearPath();
};

class MapGenerator : public QObject
{
    Q_OBJECT
public:
    void setSize(QSize);
    void generateMap();
    QVector<QSharedPointer<Tile>> returnMap();
private:
    QSize gen_size;
    QVector<QSharedPointer<Tile>> gen_tiles;
signals:
    void mapCreated();
};


#endif // USERMAP_H
