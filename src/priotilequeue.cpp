#include "priotilequeue.hpp"
#include "astar.hpp"
#include <stdlib.h>
#include <iostream>

/* adds a Tile to its place in the queue */
void PrioTileQueue::push(Tile* t)
{
    t->estimateDistance_ = abs(t->position_.x - target_->position_.y) + abs(t->position_.y - target_->position_.y);

    for(auto it = PrioQueue_.begin(); it != PrioQueue_.end(); it++){
        if(compare(*it, t) == 1 || compare(*it, t) == 0){
            PrioQueue_.insert(it, t);
            return;
        }
    }
    PrioQueue_.push_back(t);
}

/* removes and returnes the first Tile of the queue */
Tile* PrioTileQueue::pop()
{
    if(PrioQueue_.empty()) return nullptr;

    Tile* tmp(PrioQueue_.front());
    PrioQueue_.pop_front();
    return tmp;
}

/* reranges a tile in the queue */
void PrioTileQueue::update(Tile* t)
{
    for(auto it = PrioQueue_.begin(); it != PrioQueue_.end(); it++){
            if(*it == t){
                push(pop());
            }
    }
}

/* returnes the front tile of the queue */
Tile* PrioTileQueue::front()
{
    return PrioQueue_.front();
}


bool PrioTileQueue::empty() const
{
    return PrioQueue_.empty();
}

/* compare function for to tiles */
std::size_t PrioTileQueue::compare(const Tile* t1, const Tile* t2) const
{
  int diff = (t1->distance_ + t1->estimateDistance_) - (t2->distance_ + t2->estimateDistance_);
  if(diff < 0)	return -1;
  if(diff > 0)	return 1;

  return 0;
}

void PrioTileQueue::print() const
{
    std::cout << "prioQueue :";
    for(auto it = PrioQueue_.begin(); it != PrioQueue_.end(); it++){
        std::cout << (*it)->distance_ + (*it)->estimateDistance_ << " ";
    }
    std::cout << std::endl;

}

