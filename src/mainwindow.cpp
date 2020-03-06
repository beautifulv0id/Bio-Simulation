#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include "creature.hpp"
#include "datamanager.hpp"


MainWindow::MainWindow(QWidget *parent, DataManager* dataManager) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dataManager_(*dataManager),
    simArea_(this, *dataManager)
{
    ui->setupUi(this);
    setUpGui();
}



void MainWindow::setUpGui()
{
    this->setWindowTitle("BioSim");
    this->setMinimumHeight(600);
    this->setMinimumWidth(900);

    connect(ui->creatureComboBox, SIGNAL(currentIndexChanged(int)), &simArea_, SLOT(update_creature_ID(int)));
    connect(ui->placeButton, SIGNAL(clicked(bool)), &simArea_, SLOT(placeCreature()));

    connect(ui->stepButton, SIGNAL(clicked(bool)), &simArea_, SLOT(nextKIStep()));
    connect(ui->startButton, SIGNAL(clicked(bool)), &simArea_, SLOT(startTimer()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), &simArea_, SLOT(stopTimer()));

    for(int i = 0; i < dataManager_.getNumCreatures(); i++){
        QString item(QString::fromStdString(dataManager_.getCreatureModelFromID(i).name_));
        ui->creatureComboBox->addItem(item);
    }
    updateCreatureContent(0);

    QSize adjustSize(32*WIDTH, 32*HEIGHT);
    simArea_.setMinimumSize(adjustSize);
    ui->scrollArea_->setWidget(&simArea_);
    ui->scrollArea_->setVisible(true);
}

void MainWindow::updateCreatureContent(int index)
{
    const Creature& c = dataManager_.getCreatureModelFromID(index);
    QString strength(QString::number(c.strength_));
    QString velocity(QString::number(c.velocity_));
    QString life(QString::number(c.life_));
    QString attributes(QString::fromStdString(c.attributes_));

    ui->strengthLineEdit->setText(strength);
    ui->velocityLineEdit->setText(velocity);
    ui->lifeLineEdit->setText(life);
    ui->attributesTextEdit->setText(attributes);
}

void MainWindow::on_creatureComboBox_currentIndexChanged(int index)
{
    updateCreatureContent(index);
}


void MainWindow::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
}

void MainWindow::on_pauseButton_clicked()
{
    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);

}


MainWindow::~MainWindow()
{
    delete ui;
}
