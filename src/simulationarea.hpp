#ifndef INCLUDED__BIOAPPLICATION__SIMULATIONAREA__HPP
#define INCLUDED__BIOAPPLICATION__SIMULATIONAREA__HPP
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QString>
#include <vector>
#include <unordered_map>
#include "utils.hpp"
#include "creature.hpp"
#include "datamanager.hpp"
#include "ki.hpp"

/**
 *****************************************************************************
 *
 *  This class draws the simulation content
 *
 *****************************************************************************/

class CreatureInstanz;

class SimulationArea : public QWidget
{
    Q_OBJECT
public:
    SimulationArea(QWidget* parent, DataManager& dataManager);

public slots:
    void update_creature_ID(int id);
    void placeCreature();
    void nextKIStep();
    void startTimer();
    void stopTimer();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    DataManager& dataManager_;

    std::unique_ptr<const std::list<Tile*>> tilePath_;

    const QPixmap& cursorImg_;
    const QPixmap& pathImg_;


    Vec2D fromCursorPos_;
    Vec2D toCursorPos_;

    bool fromSet_;
    bool displayPath_;
    int CurrentCreatureID_;
    int cursorx_, cursory_;

    QTimer timer;

    void calculatePath();
};



#endif // INCLUDED__BIOAPPLICATION__SIMULATIONAREA__HPP
