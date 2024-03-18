#ifndef USERMAP_H
#define USERMAP_H

#include <vector>
#include <array>
#include <memory>

struct Point
{
    bool operator==(const Point& right);

    int x;
    int y;

};

struct MapSize
{
    unsigned width;
    unsigned height;
};

struct MOVE
{
    int delta_x;
    int delta_y;
};

enum {UP, RIGHT, DOWN, LEFT};

class Tile
{
public:
    Tile() = delete;
    Tile(int x, int y, bool wall =false);

    Point getCoords();
    setCoords(const Point&);

    void setWall(bool);
    bool isWall();

    std::array<Tile*, 4> neighbors = {nullptr,nullptr,nullptr,nullptr};
    // std::array<Tile*, 8> neighbors;
private:
    Point m_coords;
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
    UserMap(unsigned width, unsigned height);
    ~UserMap(){}

    unsigned getWidth();
    unsigned getHeight();
    std::vector<std::shared_ptr<Tile>> m_tiles;
private:
    MapSize m_size;
    void connectMap();


};




#endif // USERMAP_H
