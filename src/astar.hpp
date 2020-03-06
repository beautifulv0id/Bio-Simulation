#ifndef INCLUDED__BIOAPPLICATION__ASTAR__HPP
#define INCLUDED__BIOAPPLICATION__ASTAR__HPP
#include <vector>
#include <list>
#include <cmath>
#include <unordered_map>
#include <map>
#include "utils.hpp"

/**
 *****************************************************************************
 *
 *  This class is used to calculate a path between two tiles
 *
 *****************************************************************************/

class AStar
{
public:
    AStar(const std::vector<Tile*>& terrain) : terrain_(terrain){}
    std::unique_ptr<std::list<Tile*>> getShortestPath(Tile* from, Tile* to) const;

private:
    const std::vector<Tile*>& terrain_;
    const std::list<Tile*> getNeighbores(Tile* t) const;
    void populateAStar(Tile* from, Tile* to) const;
    void resetTileState() const;
};


#endif // INCLUDED__BIOAPPLICATION__ASTAR__HPP
