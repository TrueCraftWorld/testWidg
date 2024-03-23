#ifndef USERMAP_H
#define USERMAP_H

#include <vector>
#include <array>
#include <memory>
#include <QPoint>

#include <QSize>



enum {UP, RIGHT, DOWN, LEFT};

class Tile
{
public:
    Tile() = delete;
    Tile(int x, int y, bool wall =false);

    QPoint getCoords();
    setCoords(const QPoint&);

    void setWall(bool);
    bool isWall();

    std::array<Tile*, 4> neighbors = {nullptr,nullptr,nullptr,nullptr};
    // std::array<Tile*, 8> neighbors;
private:
    QPoint m_coords;
    bool m_wall;

public:
    Tile* getPrevious();
    void setPrevious(Tile* prev);
private:
    Tile* m_previous = nullptr; //inhereting simple tile to add backtracking
    // std::array<MOVE, 4> m_directions = {0,1},{1,0},{0,-1},{-1,0};
    // std::array<MOVE, 8> Directions = {{0,1},{1,0},{0,-1},{-1,0},
    //                                   {-1,1},{1,1},{1,-1},{-1,-1}};
};


class UserMap
{
public:
    UserMap(int width, int height);
    ~UserMap(){}

    int getWidth();
    int getHeight();
    std::vector<std::shared_ptr<Tile>> m_tiles;
private:
    QSize m_size;
    void connectMap();


};




#endif // USERMAP_H
