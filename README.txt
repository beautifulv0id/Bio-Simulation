DESCRIPTION
This project contains an interactive biology simulation. The goal of this simulation is to 
create a functional ecosystem based on different creatures (e.g. gras, trees, cows, fishes, tigers).
The user can choose from a list of given creatures and place them on a virtual, randomly 
generated environment. During the simulation the creatures behave according to a simple AI. 
For example, plants only reproduce if there are enough plants of the same kind in their 
neighborhood. Animals can wander around and rest afterwards or search for food in case they 
are hungry. Finally, a restricted lifetime requires them to search for a breeding mate.

COMPILE & RUN
The project has to be compiled using QMake. 
Open up the './src/Bio-Simulation.pro' file in QT Creator. 
Set the working directory to './data'.
Pass 'creature_list.txt' as command line argument.
Run.

This project was part of the 'Bachelor-Praktikum' course at the University of Bayreuth in 2017.
