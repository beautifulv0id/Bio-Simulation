#include "datamanager.hpp"
#include <iostream>
#include <QPixmap>
#include <QDir>
#include <vector>
#include <list>
#include "utils.hpp"
#include "creature.hpp"
#include "astar.hpp"
#include "tga.hpp"
#include "myexceptions.hpp"
#include "open-simplex-noise.hpp"
#include "creatureinstanz.hpp"
#include "ki.hpp"

DataManager::DataManager(const std::string &path) :
    creatureModels_(Creature::loadCreaturesFromFile(path)),
    astar(new AStar(terrainVec_))
{  
    try{
        std::shared_ptr<TGA> cursorTGA(TGA::readTGA("cursor.tga"));
        cursorImg_ = cursorTGA->toQPixmap();
        std::shared_ptr<TGA> pathTGA(TGA::readTGA("path.tga"));
        pathImg_ = pathTGA->toQPixmap();
    }catch(const TGALoadException& tgale){
        std::cout << tgale.what() << std::endl;
        exit(1);
    }

    /* load creature TGAs form creatureList into creatureImages vector */
    for(auto it = creatureModels_.begin(); it != creatureModels_.end(); it++){
        try{
            std::shared_ptr<TGA> animalTGA(TGA::readTGA((it)->imagePath_));
            QPixmap creaturePMap = animalTGA->toQPixmap();
            creatureImgs_.push_back(creaturePMap);
        }catch(const TGALoadException& tgale){
            std::cout << tgale.what() << std::endl;
            exit(1);
        }
    }

    numCreatures = creatureModels_.size();

    /* load terrain TGAs form path "terrain/" into terrainImages vector */
    QDir terrainDir("terrain/", "*.tga");
    QStringList terrainList(terrainDir.entryList());
    QString dir("terrain/");
    QString filePath;
    for(auto it : terrainList){
        filePath = dir + it;

        try{
            std::shared_ptr<TGA> terrainTGA(TGA::readTGA(filePath.toStdString()));
            QPixmap terrainPMap(terrainTGA->toQPixmap());
            terrainImgs_.push_back(terrainPMap);
        }catch(const TGALoadException& tgale){
            std::cout << tgale.what() << std::endl;
            exit(1);
        }
    }

    /* seed the noise function and store the corresponding depth of the terrain in the
     * terrain map for all tiles (value of noise indicates how deep the terrain is) */
    struct osn_context *ctx;
    std::srand(std::time(0));
    open_simplex_noise(std::rand(), &ctx);

    float scale = 100;
    double nx,ny;
    float noise;
    int result = -1, step = 1;
    for(float j = 0; j < HEIGHT; j++){
        for(float i = 0; i < WIDTH; i++){
            nx = i / scale - 0.5;
            ny = j / scale - 0.5;
            noise = open_simplex_noise2(ctx, nx / FEATURE_SIZE,
                                            ny / FEATURE_SIZE);

            result = -1;
            step = 1;
            while(result == -1){
                if(noise < (-1.f + (2.f / 6.f) * (float) (step))){
                    result = step;
                }
                step++;
            }
            bool isWater = result < 3 ? true : false;
            Tile* tmp(new Tile(result, isWater, Vec2D(i, j)));
            terrainVec_.push_back(tmp);
        }
    }
}

int DataManager::getNumCreatures() const
{
    return numCreatures;
}

const Creature &DataManager::getCreatureModelFromID(int ID) const
{
    return creatureModels_.at(ID);
}

std::list<std::shared_ptr<CreatureInstanz>>& DataManager::
                        getCreatueInstanzesOnPos(int x, int y) const
{
    return terrainVec_.at(Vec2D(x, y))->instanzList_;
}

std::list<std::shared_ptr<CreatureInstanz>>& DataManager::getAllCreatures()
{
    return creatureInstanzes_;
}

void DataManager::
    removeCreatureInstanz
            (const std::list<std::shared_ptr<CreatureInstanz>>::iterator& it)
{

    getTileAt((*it)->position_)->instanzList_.remove(*it);
    creatureInstanzes_.erase(it);
}

void DataManager::updateCreatureInstanz
            (const std::shared_ptr<CreatureInstanz>& c, const Vec2D& newPos)
{
    terrainVec_.at(c->position_)->instanzList_.remove(c);
    c->position_ = newPos;
    terrainVec_.at(c->position_)->instanzList_.push_back(c);
}

void DataManager::insertCreatureInstanz(int x, int y, int ID)
{
    const Creature& creatureTemplate(creatureModels_.at(ID));
    Vec2D creaturePos(x, y);
    std::shared_ptr<CreatureInstanz> newInstanz(
                new CreatureInstanz(creatureTemplate, creaturePos));

    terrainVec_.at(creaturePos)->instanzList_.push_back(newInstanz);
    creatureInstanzes_.push_back(newInstanz);
}

void DataManager::nextStepKI()
{
    ki_->nextStep();
}

void DataManager::insertCreatureInstanz(const std::shared_ptr<CreatureInstanz>& c){
    terrainVec_.at(c->position_)->instanzList_.push_back(c);
    creatureInstanzes_.push_back(c);
}

std::unique_ptr<std::list<Tile*>> DataManager::
            getShortestPath(const Vec2D &from, const Vec2D &to) const

{
    std::unique_ptr<std::list<Tile*>> pathList;
    try{
        std::unique_ptr<std::list<Tile*>> path(
                    astar->getShortestPath(
                        terrainVec_.at(from), terrainVec_.at(to)));

        pathList = std::move(path);
    }catch(const AStarException& ase){
        std::cout << ase.what() << std::endl;
    }

    return pathList;
}

Tile* DataManager::getTileAt(int x, int y) const
{
    Vec2D pos(x,y);
    return terrainVec_.at(pos);
}

Tile* DataManager::getTileAt(const Vec2D& pos) const
{
    return terrainVec_.at(pos);
}




const QPixmap &DataManager::getAnimalImageAt(int pos) const
{
    return creatureImgs_.at(pos);
}

const QPixmap &DataManager::getTerrainImageAt(int pos) const
{
    return terrainImgs_.at(pos);
}

const QPixmap &DataManager::getCursorImage() const
{
    return cursorImg_;
}

const QPixmap &DataManager::getPathImage() const
{
    return pathImg_;
}
