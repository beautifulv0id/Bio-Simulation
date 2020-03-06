#include "simulationarea.hpp"
#include <iostream>
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QString>
#include <QStaticText>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include "utils.hpp"
#include "creature.hpp"
#include "creatureinstanz.hpp"
#include "tga.hpp"
#include "myexceptions.hpp"
#include "open-simplex-noise.hpp"
#include "astar.hpp"
#include "datamanager.hpp"

SimulationArea::SimulationArea(QWidget *parent, DataManager& dataManager) :
    QWidget(parent),
    dataManager_(dataManager),
    cursorImg_(dataManager_.getCursorImage()),
    pathImg_(dataManager_.getPathImage()),
    fromSet_(false),
    displayPath_(false)

{
    cursorx_ = 0;
    cursory_ = 0;

    timer.setInterval(50);
    connect(&timer, SIGNAL(timeout()), this, SLOT(nextKIStep()));
}


void SimulationArea::placeCreature()
{

    int topLeftX = (int) std::abs(cursorx_) / 32;
    int topLeftY = (int) std::abs(cursory_) / 32;

    bool terrainIsWater = dataManager_.getTileAt(topLeftX, topLeftY)->isWater_;

    const Creature& currentCreature = dataManager_.getCreatureModelFromID(CurrentCreatureID_);

    if(currentCreature.water_ != terrainIsWater){
        QString creatTerrain = currentCreature.water_ == true ? "Wasserbewohner" : "Landbewohner";
        QString tileTerrain = terrainIsWater == false ? "Land" : "Wasser";
        QMessageBox::information(
            this,
            tr("BioSim"),
            "Du kannst keine " + creatTerrain + " auf dem " + tileTerrain + "plazieren.");
        return;
    }

    dataManager_.insertCreatureInstanz(topLeftX, topLeftY, CurrentCreatureID_);
    repaint();
}

void SimulationArea::nextKIStep(){
    dataManager_.nextStepKI();
    repaint();
}

void SimulationArea::startTimer(){
    timer.start();
}

void SimulationArea::stopTimer(){
    timer.stop();
}


void SimulationArea::update_creature_ID(int id)
{
    CurrentCreatureID_ = id;
}

void SimulationArea::mousePressEvent(QMouseEvent *event)
{
    /* store the position of the top left corner
     * of the tile clicked to paint it correctly */
    cursorx_ = event->pos().x()/ 32 * 32;
    cursory_ = event->pos().y()/ 32 * 32;

    if(!fromSet_){
        fromCursorPos_ = Vec2D(event->pos().x()/ 32, event->pos().y()/ 32);
        fromSet_ = true;
        displayPath_ = false;
    }
    else{
        toCursorPos_ = Vec2D(event->pos().x()/ 32, event->pos().y()/ 32);
        fromSet_ = false;
        calculatePath();
    }

    repaint();
}

void SimulationArea::calculatePath()
{

    std::unique_ptr<const std::list<Tile*>> path(dataManager_.getShortestPath(fromCursorPos_, toCursorPos_));
    if(path != nullptr && !path->empty()){
        tilePath_ = std::move(path);
        displayPath_ = true;
    }
}

/* paints the visible components to the simulationarea */
void SimulationArea::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    int windowX = parentWidget()->size().width();
    int windowY = parentWidget()->size().height();

    int visibleTilesX = ceil(float (windowX) / 32.f) + 1;
    int visibleTilesY = ceil(float (windowY) / 32.f) + 1;

    int topLeftTileX = (int) (std::abs(pos().x()) / 32);
    int topLeftTileY = (int) (std::abs(pos().y()) / 32);

    int terrainID;
    int drawX, drawY;
    /* iterates over all visible Tiles and Creatures and draws them */
    for(int i = 0; i < visibleTilesX; i++){
        for(int j = 0; j < visibleTilesY; j++){
            if((topLeftTileY+j) < HEIGHT && (topLeftTileX + i) < WIDTH){
                //draw terrain first
                drawX = (topLeftTileX*32) + i * 32;
                drawY = (topLeftTileY*32) + j * 32;
                terrainID = dataManager_.getTileAt(topLeftTileX + i, topLeftTileY + j)->ID_ - 1;
                const QPixmap& terrainToDraw = dataManager_.getTerrainImageAt(terrainID);
                painter.drawPixmap(drawX, drawY, 32, 32, terrainToDraw);

                // then draw creatures
                std::list<std::shared_ptr<CreatureInstanz>>& instanzList
                        (dataManager_.getCreatueInstanzesOnPos(topLeftTileX + i,topLeftTileY + j));

                for(std::shared_ptr<CreatureInstanz>& c : instanzList){
                    painter.drawPixmap(drawX, drawY, 32, 32,
                                       dataManager_.getAnimalImageAt(c->getID()));
                }
            }
        }
    }

     /* draw cursor if in scope of simulation Area */
    if(cursorx_ / 32 < (topLeftTileX + visibleTilesX) &&
                cursory_ / 32 < (topLeftTileY + visibleTilesY))
        painter.drawPixmap(cursorx_, cursory_, 32, 32, cursorImg_);

    /* draw astar path if set */
    if(displayPath_ && tilePath_ != nullptr){
        for(Tile* t : *tilePath_){
            painter.drawPixmap(t->position_.x*32, t->position_.y*32, 32, 32, pathImg_);
        }
    }
}


