#include <QApplication>
#include <iostream>
#include "mainwindow.hpp"
#include "datamanager.hpp"
#include "ki.hpp"

int main(int argc, char *argv[])
{
    if(argc != 2){
        std::cout << "Error: start programm like ./BioApplication <creaturetable path>" << std::endl;
        return EXIT_FAILURE;
    }
    QApplication a(argc, argv);
    int retval;

    try{
        DataManager dataManager(argv[1]);
        KI ki(dataManager);
        dataManager.setKI(&ki);
        MainWindow w(nullptr, &dataManager);
        w.show();
        retval = a.exec();
    }catch(const std::exception& e){
        std::cout << e.what() << std::endl;
        exit(1);
    }


    return retval;
}
