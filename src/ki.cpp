#include "ki.hpp"

#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <iostream>

#include "creature.hpp"
#include "creatureinstanz.hpp"
#include "utils.hpp"
#include "myexceptions.hpp"
#include "astar.hpp"
#include "datamanager.hpp"

void KI::nextStep()
{   

    for(auto it = dataManager_.getAllCreatures().begin();
                    it != dataManager_.getAllCreatures().end(); ){
        auto itSave = it;
        it++;
        remove = false;
        std::shared_ptr<CreatureInstanz> c(*itSave);
        if(c->isAnimal()) nextStepAnimal(c);
        else nextStepPlant(c);
        c->steps_++;

        if(remove == true){
            dataManager_.removeCreatureInstanz(itSave);
        }

    }
}

void KI::nextStepAnimal(const CreatureInstanz::sptr& c)
{
    if(c->life_ <= 0) {remove = true; return; }
    else if(c->state_ == CreatureInstanz::wandern) nextStepWandern(c);
    else if(c->state_ == CreatureInstanz::paarung) nextStepPaaren(c);
    else if(c->state_ == CreatureInstanz::jagen) nextStepJagen(c);
    else if(c->state_ == CreatureInstanz::angriff) nextStepAngriff(c);
    else if(c->state_ == CreatureInstanz::ausruhen) nextStepAusruhen(c);
    c->stepsMating_++;
}

void KI::nextStepWandern(const CreatureInstanz::sptr& c)
{
    int startLife = c->getStartLife();

    if(c->life_ > startLife / 2 &&
            c->stepsMating_ > c->waitForMating_ &&
                                searchForOthers(c))
    {
        c->state_ = CreatureInstanz::paarung;
    }
    else if(c->life_ < (startLife * 0.6) && searchForQuerry(c))
    {
        c->state_ = CreatureInstanz::jagen;
    }
    else Wandern(c);
}

void KI::Wandern(const CreatureInstanz::sptr& c)
{
    if(c->path_->empty()) calcNextTarget(c);

    goToNextTile(c);

    if(c->path_->empty()){
        c->state_ = CreatureInstanz::ausruhen;
        c->steps_ = 0;
        c->restSteps_ = c->life_ / c->getStartLife() * 5;
    }

    c->life_ -= 2;
}

void KI::calcNextTarget(const CreatureInstanz::sptr& c)
{
    Tile* to(randWalkTile(c));
    if(to != nullptr)
        c->path_ = dataManager_.getShortestPath(c->position_, to->position_);
}

void KI::goToNextTile(const CreatureInstanz::sptr& c)
{
    unsigned int nextTileIndex =
            static_cast<unsigned int> (c->getVelocity() / 20 - 1) < (c->path_->size()) ?
                (c->getVelocity() / 20 - 1) : c->path_->size() - 1;

    /* calculate next tile */
    while(nextTileIndex > 0 && !c->path_->empty()){
        c->path_->pop_front();
        nextTileIndex--;
    }

    if(!c->path_->empty()){
        Vec2D newPos = c->path_->front()->position_;
        dataManager_.updateCreatureInstanz(c, newPos);
        c->path_->pop_front();
    }


}

void KI::nextStepPaaren(const CreatureInstanz::sptr& c)
{
    placeNewCreatures(c);
    c->life_ = c->life_ * 3 / 4;
    c->state_ = CreatureInstanz::wandern;
    c->stepsMating_ = 0;
    c->waitForMating_ = c->life_ / 50;
}

void KI::nextStepJagen(const CreatureInstanz::sptr& c)
{

    if(c->querry_.get() != nullptr && querryReachable(c)){
        c->path_->clear();
        c->state_ = CreatureInstanz::angriff;
    }
    if(c->path_->empty())
        searchForQuerry(c);
    else goToNextTile(c);
}

void KI::nextStepAngriff(const CreatureInstanz::sptr& c)
{
    if(c->querry_->life_ <= 0){
        c->state_ = CreatureInstanz::ausruhen;
        c->steps_ = 0;
        c->restSteps_ = c->life_ / c->getStartLife() * 5;
    }
    if(c->querry_.get() != nullptr && querryReachable(c)){
        c->querry_->life_ -= c->getStrength();
        c->life_ += c->getStrength();
    }
    else{
        c->state_ = CreatureInstanz::jagen;
        c->querry_.reset();
    }
}

void KI::nextStepAusruhen(const CreatureInstanz::sptr& c)
{
    if(c->steps_ >= c->restSteps_) c->state_ = CreatureInstanz::wandern;
    else c->life_ -= 5;
}

void KI::nextStepPlant(const CreatureInstanz::sptr& c)
{
    if(c->life_ <= 0) {remove = true; return; }
    else if(c->state_ == CreatureInstanz::warten) nextStepWarten(c);
    else if(c->state_ == CreatureInstanz::wachsen)nextStepWachsen(c);
}

void KI::nextStepWarten(const CreatureInstanz::sptr& c)
{
    if(c->steps_ > c->life_ / 100 && searchForOthers(c)){
        c->state_ = CreatureInstanz::wachsen;
        c->steps_ = 0;
    }
    else{
        if(dataManager_.getTileAt(c->position_)->ID_ > 1
                && dataManager_.getTileAt(c->position_)->ID_ < 5)
            c->life_ -= 10;
        else c->life_ -= 25;
    }
}

void KI::nextStepWachsen(const CreatureInstanz::sptr& c)
{
    placeNewCreatures(c);
    c->state_ = CreatureInstanz::warten;
    c->steps_ = 0;
}


bool KI::searchForOthers(const CreatureInstanz::sptr& c){
    int searchWidth = c->isAnimal() ? 3 : 5;

    int borderLeft =    c->position_.x - searchWidth >= 0       ?
                c->position_.x - searchWidth  : 0;

    int borderTop =     c->position_.y - searchWidth >= 0       ?
                c->position_.y - searchWidth  : 0;

    int borderRight =   c->position_.x + searchWidth < WIDTH    ?
                c->position_.x + searchWidth  : WIDTH - 1;

    int borderBottom =  c->position_.y + searchWidth < HEIGHT   ?
                c->position_.y + searchWidth  : HEIGHT - 1;

    int creatCount = 0;

    /* Count creatures with same ID in the frame calcualted */
    for(int i = borderLeft; i < borderRight; i++){
        for(int j = borderTop; j < borderBottom; j++){
            for(std::shared_ptr<CreatureInstanz>& ct : dataManager_.getTileAt(i, j)->instanzList_){
                if(ct->getID() == c->getID() && ct.get() != c.get()) creatCount++;
            }
        }
    }

    int min = c->isAnimal() ? 1 : 2;
    int max = c->isAnimal() ? 5 : 10;
    if(creatCount > min && creatCount < max){
        return true;
    }

    return false;
}

void KI::placeNewCreatures(const CreatureInstanz::sptr& c)
{
    int searchWidth = c->isAnimal() ? 3 : 5;
    int trys = 0, count = 0, randX, randY, plantX, plantY;
    bool rand, noOtherCreat;
    Vec2D plantPos;
    std::srand(std::time(0));

    while(trys < 100 && count < 2){
        rand = false;

        /* search for a appropriate tile in the search width and save it in plantPos */
        while(!rand){
            randX = (std::rand() % searchWidth);
            randY = (std::rand() % searchWidth);

            randX = (std::rand() % 2) == 0 ? -1 * randX : randX;
            randY = (std::rand() % 2) == 0? -1 * randY : randY;

            plantX = c->position_.x + randX;
            plantY = c->position_.y + randY;

            if(plantX >= WIDTH) plantX = WIDTH - 1;
            else if(plantX < 0) plantX = 0;
            if(plantY >= HEIGHT) plantY = HEIGHT - 1;
            else if(plantY < 0) plantY = 0;

            plantPos = Vec2D(plantX, plantY);

            if(plantPos != c->position_){
                rand = true;
            }

            trys++;
            if(trys >= 100)
                return;
        }

        noOtherCreat = true;

        /* if current creature is land / water and no other creature is on plantPos, add new creature */
        if(c->isLand()){
            if(dataManager_.getTileAt(plantPos)->isLand()){

                if(!dataManager_.getTileAt(plantPos)->instanzList_.empty()) noOtherCreat = false;
                if(noOtherCreat){
                    std::shared_ptr<CreatureInstanz> newInstanz
                            (new CreatureInstanz(dataManager_.getCreatureModelFromID(c->getID()), plantPos));
                    if(newInstanz->isAnimal()) newInstanz->life_ = c->life_ / 2;
                    dataManager_.insertCreatureInstanz(newInstanz);
                    count++;
                }
            }
        }
        else if(c->isWater()){
            if(dataManager_.getTileAt(plantPos)->isWater()){
                if(!dataManager_.getTileAt(plantPos)->instanzList_.empty()) noOtherCreat = false;
                if(noOtherCreat){
                    CreatureInstanz::sptr newInstanz
                            (new CreatureInstanz(dataManager_.getCreatureModelFromID(c->getID()), plantPos));
                    if(newInstanz->isAnimal()) newInstanz->life_ = c->life_ / 2;
                    dataManager_.insertCreatureInstanz(newInstanz);
                    count++;
                }
            }
        }
        trys++;
    }

}


Tile* KI::randWalkTile(const CreatureInstanz::sptr& c)
{
    int trys = 0, randX, randY, walkX, walkY;
    Vec2D walkPos;
    bool rand;
    std::srand(std::time(0));
    while(trys < 100){
        rand = false;

        /* search for a appropriate tile in the walking range and save it in plantPos */
        while(!rand){
            randX = (std::rand() % 11);
            randY = (std::rand() % 11);
            walkX = c->position_.x + randX - 5 < 0 ? 0 : c->position_.x + randX - 5;
            walkY = c->position_.y + randY - 5 < 0 ? 0 : c->position_.y + randY - 5;
            if(walkX >= WIDTH) walkX = WIDTH - 1;
            if(walkY >= HEIGHT) walkY = HEIGHT - 1;

            walkPos = Vec2D(walkX, walkY);
            if(walkPos != c->position_){
                rand = true;
            }

            trys++;
            if(trys >= 100)
                return nullptr;
        }
        if(c->isLand()){
            if(dataManager_.getTileAt(walkPos)->isLand()){
                return dataManager_.getTileAt(walkPos);
            }
        }
        else{
            if(dataManager_.getTileAt(walkPos)->isWater()){
                return dataManager_.getTileAt(walkPos);
            }
        }
        trys++;
    }

    return nullptr;
}

bool KI::searchForQuerry(const CreatureInstanz::sptr& c)
{
    int borderLeft =    c->position_.x - 5 >= 0     ? c->position_.x - 5 : 0;
    int borderTop =     c->position_.y - 5 >= 0     ? c->position_.y - 5 : 0;
    int borderRight =   c->position_.x + 5 < WIDTH  ? c->position_.x + 5: WIDTH - 1;
    int borderBottom =  c->position_.y + 5 < HEIGHT ? c->position_.y + 5 : HEIGHT - 1;

    for(int i = borderLeft; i < borderRight; i++){
        for(int j = borderTop; j < borderBottom; j++){

            for(std::shared_ptr<CreatureInstanz>& other : dataManager_.getTileAt(i, j)->instanzList_){

                if(other->getID() != c->getID()){
                    /* land creatures */
                    if(c->isLand()){
                        if(c->isCarnivoire()){
                            /* if current is a land creature and a carnivoire and other is a land animal,
                             * add other to querry, update path and return true */
                            if(other->isAnimal() && other->isLand()){
                                c->querry_ = other;
                                c->path_ = dataManager_.getShortestPath(c->position_, other->position_);
                                return true;
                            }
                        }
                        else if(c->isHerbivore()){
                            /* if current is a land creature and a herbivoire and other is a rooted plant,
                             * add other to querry, update path and return true */
                            if(other->isPlant() && other->isLand()){
                                c->querry_ = other;
                                c->path_ = dataManager_.getShortestPath(c->position_, other->position_);
                                return true;
                            }
                        }
                    }
                    /* water creatures */
                    else if(c->isWater()){
                        if(c->isCarnivoire()){
                            /* if current is a water creature and a carnivoire and other is a water animal,
                             * add other to querry, update path and return true */
                            if(other->isAnimal() && other->isWater()){
                                c->querry_ = other;
                                c->path_ = dataManager_.getShortestPath(c->position_, other->position_);
                                return true;
                            }
                        }
                        else if(c->isHerbivore()){
                            /* if current is a water creature and a herbivoire and other is a water plant,
                             * add other to querry, update path and return true */
                            if(other->isPlant() && other->isWater()){
                                c->querry_ = other;
                                c->path_ = dataManager_.getShortestPath(c->position_, other->position_);
                                return true;
                            }
                        }
                    }
                }

            }

        }
    }

    return false;
}

bool KI::querryReachable(const CreatureInstanz::sptr& c)
{
    int borderLeft =    c->position_.x - 1 >= 0     ? c->position_.x - 1 : 0;
    int borderTop =     c->position_.y - 1 >= 0     ? c->position_.y - 1 : 0;
    int borderRight =   c->position_.x + 1 < WIDTH  ? c->position_.x + 1: WIDTH - 1;
    int borderBottom =  c->position_.y + 1 < HEIGHT ? c->position_.y + 1 : HEIGHT - 1;

    for(int x = borderLeft; x < borderRight; x++){
        for(int y = borderTop; y < borderBottom; y++){
            if(Vec2D(x, y) == c->querry_->position_) return true;
        }
    }
    return false;
}



