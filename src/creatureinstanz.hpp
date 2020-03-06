#ifndef INCLUDED__BIOAPPLICATION__CREATUREINSTANZ__HPP
#define INCLUDED__BIOAPPLICATION__CREATUREINSTANZ__HPP
#include <string>
#include <list>
#include "creature.hpp"
#include "utils.hpp"
#include "datamanager.hpp"

/**
 **********************************************************************************
 *
 *  This class represents a Creature Instanz
 *
 **********************************************************************************/

class CreatureInstanz
{
public:
    typedef std::shared_ptr<CreatureInstanz> sptr;
    enum creatureState{warten, wachsen, wandern, paarung, jagen, angriff, ausruhen};

    CreatureInstanz(const Creature& c, Vec2D position) :
        creatureModel_(&c),
        life_(c.life_),
        position_(position),
        path_(new std::list<Tile*>())
    {
        if(creatureModel_->type_ == Creature::plant) state_ = warten;
        else state_ = wandern;
        steps_ = restSteps_ = stepsMating_ = 0;
        waitForMating_ = life_ / 50;
    }

    const Creature* creatureModel_;
    int life_;
    Vec2D position_;

    std::unique_ptr<std::list<Tile*>> path_;
    CreatureInstanz::sptr querry_;
    creatureState state_;
    int steps_;
    int restSteps_;
    int stepsMating_;
    int waitForMating_;

    bool isAnimal(){ return creatureModel_->type_ != Creature::creatureType::plant; }
    bool isPlant(){ return creatureModel_->type_ == Creature::creatureType::plant; }
    bool isWater(){ return creatureModel_->water_ == true; }
    bool isLand(){ return creatureModel_->water_ == false; }
    bool isCarnivoire() { return creatureModel_->type_ == Creature::creatureType::carnivore;}
    bool isHerbivore() { return creatureModel_->type_ == Creature::creatureType::herbivore;}

    int getID() const { return creatureModel_->ID_; }
    int getVelocity() const { return creatureModel_->velocity_; }
    int getStrength() const { return creatureModel_->strength_; }
    int getStartLife() const { return creatureModel_->life_;}
    Creature::creatureType getCreatureType() const { return creatureModel_->type_; }
};

#endif // INCLUDED__BIOAPPLICATION__CREATUREINSTANZ__HPP
