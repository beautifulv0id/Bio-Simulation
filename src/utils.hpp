#ifndef INCLUDED__BIOAPPLICATION__UTILS__HPP
#define INCLUDED__BIOAPPLICATION__UTILS__HPP

#include <vector>
#include <list>

static const int HEIGHT = 1000;
static const int WIDTH = 1000;
static const double FEATURE_SIZE = 0.08;
static const int MAXDIST = 300;


/**
 **********************************************************************************
 *
 *  This class describes a 2D Vector with a operator() function to hash its value
 *
 **********************************************************************************/

struct Vec2D{
    Vec2D() : x(-1), y(-1){}
    Vec2D(int x, int y) : x(x), y(y){}
    int x, y;
    bool operator ==(const Vec2D& other) const {
        if(x == other.x && y == other.y) return true;
        return false;
    }
    bool operator !=(const Vec2D& other) const {
        if(x != other.x || y != other.y) return true;
        return false;
    }
    std::size_t operator() (const Vec2D& key) const {
        size_t ret = key.x + key.y * std::max(WIDTH, HEIGHT);
        return ret;
    }
    operator int() const { return y * WIDTH + x; }
};

class Vec2DComparator{
    bool operator() (const Vec2D& a, const Vec2D& b) const {
        if(a.x == b.x && a.y == b.y) return true;
        return false;
    }
};

/**
 **********************************************************************************
 *
 * This class represents a Tile for the terrain in the simulation area and is
 * neccecary for the A* algorithm
 *
 **********************************************************************************/

class CreatureInstanz;

class Tile{

public:
    typedef std::shared_ptr<Tile> sptr;
    typedef const std::shared_ptr<Tile> c_sptr;
    enum Status {white, gray, black};

    friend class AStar;
    friend class PrioTileQueue;

    Tile(int id, bool isWater, const Vec2D& p) :
        state_(white), ID_(id), isWater_(isWater),
        position_(p), pred_(nullptr)
    {
    }

    bool operator ==(const Tile& other){
        if(position_.x == other.position_.x && position_.y == other.position_.y) return true;
        return false;
    }

    bool isLand() { return isWater_ == false; }
    bool isWater() { return isWater_ == true; }

    Status state_;
    const int ID_;
    const bool isWater_;
    const Vec2D position_;

    std::list<std::shared_ptr<CreatureInstanz>> instanzList_;

private:
    Tile* pred_;
    int distance_;
    int estimateDistance_;
};


#endif // INCLUDED__BIOAPPLICATION__UTILS__HPP
