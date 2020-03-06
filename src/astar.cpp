#include "astar.hpp"
#include <stdlib.h>
#include <iostream>
#include "priotilequeue.hpp"
#include "utils.hpp"
#include "myexceptions.hpp"
#include "datamanager.hpp"

int edgeWeight(int id1, int id2);

std::unique_ptr<std::list<Tile*>> AStar::getShortestPath(Tile* from,Tile* to) const
{
    std::unique_ptr<std::list<Tile*>> path(new std::list<Tile*>());

    if(from == nullptr || to == nullptr){
        return path;
    }

    populateAStar(from, to);

    /* if from == to return to */
    if (to->pred_ == nullptr) {
        if(from == to){
            path->push_back(to);
        }
        return path;
    }

    /* fill path with tiles in right order */
    Tile* reverseTile(to);
    while(reverseTile->pred_ != nullptr && reverseTile->position_ != from->position_){
        path->push_front(reverseTile);
        reverseTile->state_ = Tile::black;
        reverseTile = reverseTile->pred_;
    }
    path->push_front(reverseTile);

    return path;
}

void AStar::populateAStar(Tile* from, Tile* to) const
{
    resetTileState();

    PrioTileQueue distQueue(to);
    from->state_ = Tile::gray;
    from->distance_ = 0;
    from->pred_ = nullptr;
    distQueue.push(from);

    while(!distQueue.empty()){
        Tile* current(distQueue.pop());
        current->state_ = Tile::black;
        if(current->distance_ > MAXDIST){
            std::cout << "Ziel unerreichbar" << std::endl;
            throw AStarException();
        }
        if(current == to){
            return;
        }

        for(Tile* neighbor : getNeighbores(current)){
            /* if neighbor hasn't been visited jet, calculate distance and
             * add it to the distance queue */
            if(neighbor->state_ == Tile::white){
                neighbor->state_ = Tile::gray;
                neighbor->distance_ = current->distance_ +
                        edgeWeight(current->ID_, neighbor->ID_);
                neighbor->pred_ = current;
                distQueue.push(neighbor);
            }
            /* if neighbor has been visited jet and newDistance is smaller
             * than the old distance update neighbor in distance queue */
            else if(neighbor->state_ == Tile::gray){
                int newDistance = current->distance_ +
                        edgeWeight(current->ID_, neighbor->ID_);
                if(newDistance < neighbor->distance_){
                    neighbor->distance_ = newDistance;
                    neighbor->pred_ = current;
                    distQueue.update(neighbor);
                }
            }
        }
    }
}



/* calculates the edge weight according to the given table */
int edgeWeight(int id1, int id2)
{
    if(id1 > 2){
        if(id2 == 3 || id2 == 4) return 1;
        if(id2 == 5) return 4;
        if(id2 == 6) return 2;
    }
    else{
        if(id2 == 1) return 3;
        if(id2 == 2) return 1;
    }

    return -1;
}


const std::list<Tile*> AStar::getNeighbores(Tile* currentTile) const
{
    std::list<Tile*> neighbors;
    int tx = currentTile->position_.x;
    int ty = currentTile->position_.y;

    Tile* tmp;
    /* check if neighbor tiles are in simulation area and can be reached
     * if so, add them to the return list */
    if(currentTile->isLand()){
        if(ty - 1 >= 0){
            tmp = terrain_.at(Vec2D(tx, ty-1));
            if(tmp->isLand())
                neighbors.push_back((tmp));
        }
        if(tx + 1 < WIDTH){
            tmp = terrain_.at(Vec2D(tx+1, ty));
            if(tmp->isLand())
                neighbors.push_back((tmp));
        }
        if(ty+1 < HEIGHT){
            tmp = terrain_.at(Vec2D(tx, ty+1));
            if(tmp->isLand())
                neighbors.push_back((tmp));
        }
        if(tx-1 >= 0){
            tmp = terrain_.at(Vec2D(tx-1, ty));
            if(tmp->isLand())
                neighbors.push_back((tmp));
        }
    }
    else if(currentTile->isWater()){
        if(ty - 1 >= 0){
            tmp = terrain_.at(Vec2D(tx, ty-1));
            if(tmp->isWater())
                neighbors.push_back((tmp));
        }
        if(tx + 1 < WIDTH){
            tmp = terrain_.at(Vec2D(tx+1, ty));
            if(tmp->isWater())
                neighbors.push_back((tmp));
        }
        if(ty+1 < HEIGHT){
            tmp = terrain_.at(Vec2D(tx, ty+1));
            if(tmp->isWater())
                neighbors.push_back((tmp));
        }
        if(tx-1 >= 0){
            tmp = terrain_.at(Vec2D(tx-1, ty));
            if(tmp->isWater())
                neighbors.push_back((tmp));
        }

    }

    return neighbors;
}

void AStar::resetTileState() const
{
    for(Tile* tile : terrain_){
        tile->state_ = Tile::white;
        tile->distance_ = MAXDIST;
        tile->pred_ = nullptr;
    }
}


