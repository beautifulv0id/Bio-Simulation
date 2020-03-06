#ifndef INCLUDED__BIOAPPLICATION__MAINWINDOW__HPP
#define INCLUDED__BIOAPPLICATION__MAINWINDOW__HPP
#include <QMainWindow>
#include <vector>
#include "creature.hpp"
#include "simulationarea.hpp"
#include "datamanager.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, DataManager* dataManager = 0);
    ~MainWindow();

private slots:
    void on_creatureComboBox_currentIndexChanged(int index);
    void on_startButton_clicked();
    void on_pauseButton_clicked();

private:
    Ui::MainWindow *ui;
    DataManager& dataManager_;
    SimulationArea simArea_;
    void setUpGui();
    void updateCreatureContent(int index);
};

#endif // INCLUDED__BIOAPPLICATION__MAINWINDOW__HPP
