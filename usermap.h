#ifndef USERMAP_H
#define USERMAP_H

#include <array>
#include <QPoint>
#include <QObject>
#include <QSize>
#include <QVector>
#include <QList>
#include <QSharedPointer>
#include <QPointer>

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

    QPointer<Tile>  getPrevious();
    void setPrevious(QPointer<Tile>  prev);

    QList<QPointer<Tile>> neighbors;

signals:
    void stateChanged();

private:
    QPoint m_coords;
    QPointer<Tile>  m_previous = nullptr; //inhereting simple tile to add backtracking
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
    QPointer<Tile> tileAt(int index);
    QPointer<Tile> tileAt(int x, int y);
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
