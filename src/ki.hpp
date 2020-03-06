#ifndef INCLUDED__BIOAPPLICATION__KI__HPP
#define INCLUDED__BIOAPPLICATION__KI__HPP
#include <vector>
#include "utils.hpp"
#include "creatureinstanz.hpp"
#include "creature.hpp"
#include "astar.hpp"
#include "datamanager.hpp"

/**
 **********************************************************************************
 *
 *  This class provides all methods for the game logic
 *
 **********************************************************************************/

class KI
{
public:
    KI(DataManager& dataManager) :
        dataManager_(dataManager)
    {
        remove = false;
    }

    void nextStep();

private:

    /* Animal KI */
    void nextStepAnimal(const CreatureInstanz::sptr& c);
    void nextStepWandern(const CreatureInstanz::sptr& c);
    void Wandern(const CreatureInstanz::sptr& c);
    void calcNextTarget(const CreatureInstanz::sptr& c);
    void goToNextTile(const CreatureInstanz::sptr& c);
    void nextStepPaaren(const CreatureInstanz::sptr& c);
    void nextStepJagen(const CreatureInstanz::sptr& c);
    void nextStepAngriff(const CreatureInstanz::sptr& c);
    void nextStepAusruhen(const CreatureInstanz::sptr& c);

    /* Plant KI */
    void nextStepPlant(const CreatureInstanz::sptr& c);
    void nextStepWarten(const CreatureInstanz::sptr& c);
    void nextStepWachsen(const CreatureInstanz::sptr& c);
    bool searchForOthers(const CreatureInstanz::sptr& c);
    void placeNewCreatures(const CreatureInstanz::sptr& c);

    /* help functions */
    Tile* randWalkTile(const CreatureInstanz::sptr& c);
    bool searchForQuerry(const CreatureInstanz::sptr& c);
    bool querryReachable(const CreatureInstanz::sptr& c);

    DataManager& dataManager_;
    bool remove;

};

#endif // INCLUDED__BIOAPPLICATION__KI__HPP
