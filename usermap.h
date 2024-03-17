#ifndef USERMAP_H
#define USERMAP_H

#include <vector>

struct Point
{
    bool operator==(const Point& right);

    unsigned x;
    unsigned y;

};

struct MapSize
{
    unsigned width;
    unsigned height;
};

enum {UP, RIGHT, DOWN, LEFT};

class Tile
{
public:
    Tile();
    Tile(unsigned x, unsigned y, bool wall);

    Point getCoords();
    setCoords(const Point&);

    void setWall(bool);
    bool isWall();

    std::vector<Tile*> neighbors;
private:
    Point m_coords;
    bool m_wall;

public:
    Tile* getPrevious();
    void setPrevious(Tile* prev);
private:
    Tile* m_previous = nullptr; //inhereting simple tile to add backtracking
};


class UserMap
{
public:
    UserMap(unsigned width, unsigned height);
    unsigned getWidth();
    unsigned getHeight();
    std::vector<Tile*> m_tiles;
private:
    MapSize m_size;
    void connectMap();


};




#endif // USERMAP_H
