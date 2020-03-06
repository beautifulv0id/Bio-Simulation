#ifndef INCLUDED__BIOAPPLICATION__PRIOTILEQUEUE__HPP
#define INCLUDED__BIOAPPLICATION__PRIOTILEQUEUE__HPP
#include <list>
#include "astar.hpp"
#include "utils.hpp"

/**
 *****************************************************************************
 *
 *  This class implements the functions of a priority queue based on the
 *  estimated distance to the target tile
 *  i.e orders the tiles from the lowes to highest estimated distance
 *
 *****************************************************************************/

class PrioTileQueue
{
public:
    PrioTileQueue(Tile* target) : target_(target){}
    Tile* pop();
    Tile* front();
    void push(Tile* t);
    void update(Tile* t);
    bool empty() const;
    std::size_t compare(const Tile* t1, const Tile* t2) const;
    void print() const;

private:
    std::list<Tile*> PrioQueue_;
    const Tile* target_;
};

#endif // INCLUDED__BIOAPPLICATION__PRIOTILEQUEUE__HPP
