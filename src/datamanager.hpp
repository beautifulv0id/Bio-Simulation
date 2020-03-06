#ifndef INCLUDED__BIOAPPLICATION__DATAMANAGER__HPP
#define INCLUDED__BIOAPPLICATION__DATAMANAGER__HPP
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <QPixmap>
#include "utils.hpp"
#include "astar.hpp"
#include "creature.hpp"

/**
 **********************************************************************************
 *
 *  This class holds all the necessary data and methods to access the data
 *
 **********************************************************************************/

class CreatureInstanz;
class AStar;
class KI;

class DataManager
{

public:
    typedef std::list<std::shared_ptr<CreatureInstanz>> creatureInstanzList;

    DataManager(const std::string& path);
    ~DataManager() {
        for(Tile* t : terrainVec_){
            delete t;
        }
    }

    void setKI(KI* ki){ ki_ = ki; }

    int getNumCreatures() const;

    const Creature& getCreatureModelFromID(int ID) const;
    std::list<std::shared_ptr<CreatureInstanz>>& getCreatueInstanzesOnPos(int x, int y) const;
    std::list<std::shared_ptr<CreatureInstanz>>& getAllCreatures();
    void removeCreatureInstanz(const std::list<std::shared_ptr<CreatureInstanz>>::iterator& it);
    void updateCreatureInstanz(const std::shared_ptr<CreatureInstanz>& c, const Vec2D& newPos);
    void insertCreatureInstanz(const std::shared_ptr<CreatureInstanz>& c);
    void insertCreatureInstanz(int x, int y, int ID);
    void nextStepKI();

    std::unique_ptr<std::list<Tile*>> getShortestPath(const Vec2D &from,const Vec2D &to) const;
    Tile* getTileAt(int x, int y) const;
    Tile* getTileAt(const Vec2D& pos) const;

    const QPixmap& getAnimalImageAt(int pos) const;
    const QPixmap& getTerrainImageAt(int pos) const;
    const QPixmap& getCursorImage() const;
    const QPixmap& getPathImage() const;

private:
    std::vector<Tile*> terrainVec_;
    std::list<std::shared_ptr<CreatureInstanz>> creatureInstanzes_;
    std::vector<Creature> creatureModels_;

    std::vector<QPixmap> creatureImgs_;
    std::vector<QPixmap> terrainImgs_;

    QPixmap cursorImg_;
    QPixmap pathImg_;

    std::unique_ptr<AStar> astar;
    KI* ki_;

    int numCreatures;
};

#endif // INCLUDED__BIOAPPLICATION__DATAMANAGER__HPP
