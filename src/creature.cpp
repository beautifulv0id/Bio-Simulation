#include "creature.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <QString>
#include "myexceptions.hpp"
#include "utils.hpp"


std::vector<std::string> splitAt(const std::string& s, char delimiter);

Creature::Creature(int ID, std::string name, int strength, int velocity,
                   int life, std::string attributes, std::string imagePath,
                   bool water, creatureType type)
    :
    ID_(ID), name_(name), strength_(strength),
    velocity_(velocity), life_(life),
    attributes_(attributes), imagePath_(imagePath),
    water_(water), type_(type)
{
}

std::vector<Creature> Creature::loadCreaturesFromFile(const std::string& pfad)
{

    std::vector<Creature> creatureVector;
    std::ifstream inputFile;
    inputFile.open(pfad, std::ios::in);
    if(!inputFile.is_open()) {
        std::cout << "Couldnt open file" << std::endl;
        throw CreatureLoadException();
    }

    int count_lines = 0;
    int count_correct_lines = 0;
    std::string line;
    while (!inputFile.eof())
    {

        std::getline(inputFile, line);
        count_lines++;

        std::vector<std::string> attributesVector = splitAt(line, ',');

        if(line.empty()) continue;

            try{
                validateInput(attributesVector);
                int ID = count_lines - 1;
                std::string name(attributesVector[0]);
                int strength = std::stoi(attributesVector[1].c_str());;
                int velocity = std::stoi(attributesVector[2].c_str());;
                int life = std::stoi(attributesVector[3].c_str());;
                std::string attributes(attributesVector[4]);
                std::string imagePath(attributesVector[5]);
                bool cWater = true;
                creatureType cType = plant;

                QString string(QString::fromStdString(attributes));
                if(string.contains("landbewohner", Qt::CaseInsensitive)) cWater = false;
                if(string.contains("pflanzenfresser", Qt::CaseInsensitive)) cType = herbivore;
                if(string.contains("fleischfresser", Qt::CaseInsensitive)) cType = carnivore;

                Creature newCreature
                            (ID, name, strength, velocity,
                             life, attributes, imagePath, cWater, cType);

                creatureVector.push_back(newCreature);
                count_correct_lines++;
            }catch(const CreatureLoadException& cle){
                std::cout << cle.what() << ", L." << count_lines << std::endl;
            }
    }

    std::cout << "\nEingelesene Zeilen:" << count_lines << std::endl;
    std::cout << "Fehlerhafte Zeilen:" << (count_lines-count_correct_lines) << std::endl;

    return creatureVector;
}



void Creature::validateInput(const std::vector<std::string>& attrv)
{
    if(attrv.size() != 6){
        std::cout << "Invalid input argument size" << std::endl;
        throw CreatureLoadException();
    }
    validateName(attrv[0]);
    validatePositiveInt(attrv[1]);
    validatePositiveInt(attrv[2]);
    validatePositiveInt(attrv[3]);
    validateAttributes(attrv[4]);
    validatePath(attrv[5]);
}

void Creature::validateName(const std::string& name)
{
    for(std::string::const_iterator it(name.begin()); it != name.end(); it++) {
        if((*it < 'A' && *it != ' ') || (*it > 'Z' && *it < 'a') || *it > 'z'){
            std::cout << "Invalid name" << std::endl;
            throw CreatureLoadException();
        }
    }
}

void Creature::validatePositiveInt(const std::string& str){
    int value = 0;
    int digit;
    std::string::const_iterator it(str.begin());

    if(*it < '0' || *it > '9'){
        std::cout << "Invalid number" << std::endl;
        throw CreatureLoadException();
    }

    digit = *it - '0';
    value = value * 10 + digit;

    if(digit == 0 && str.length()!=1){
        std::cout << "Invalid number" << std::endl;
        throw CreatureLoadException();
    }
    it++;
    for(; it != str.end(); it++) {
        if(*it < '0' || *it > '9'){
            std::cout << "Invalid number" << std::endl;
            throw CreatureLoadException();
        }

        digit = *it - '0';
        value = value * 10 + digit;

    }
}

void Creature::validateAttributes(const std::string& attr)
{
    for(std::string::const_iterator it(attr.begin()); it != attr.end(); it++) {
        if((*it < '0' && *it != ' ')||
           (*it > '9' && *it < 'A') ||
           (*it > 'Z' && *it < 'a' && *it != 'a') || *it > 'z'){
            std::cout << "Invalid attributes" << std::endl;
            throw CreatureLoadException();
        }
    }
}

void Creature::validatePath(const std::string& pfad)
{
    int state = 0;

    /* Automat that validates the path. Cycles thorough each character.
     *
     * case 0: check for at least one valid character at beginning of each sub folder
     * (yes -> case 1)
     * case 1: check for valid characters ( '/' -> case 0); ('.' -> case 2)
     * case 2: check for at least one valid character at data ending (yes -> case 3)
     * case 3: check for valid characters at data ending (yes -> state 3)
     *
     * if any check fails throw exception
     * */
    for(std::string::const_iterator it(pfad.begin()); it != pfad.end(); it++){
        switch (state) {
        case 0:
            if((*it >= 'A' && *it <= 'Z') || *it == '_' || (*it >= 'a' && *it <= 'z'))
                state = 1;
            else
                state = -1;

            break;
        case 1:
            if((*it >= 'A' && *it <= 'Z') || *it == '_' || (*it >= 'a' && *it <= 'z'))
                state = 1;

            else if(*it == '/') state = 0;
            else if(*it == '.') state = 2;

            else
                state = -1;

            break;
        case 2:
            if((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z'))
                state = 3;
           else
                state = -1;

            break;
        case 3:
            if((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z'))
                state = 3;
            else
                state = -1;

            break;
        default:
            std::cout << "Invalid path" << std::endl;
            throw CreatureLoadException();
            break;
        }
    }
}

/* Splits a string at given delimiter */
std::vector<std::string> splitAt(const std::string& s, char delimiter)
{
    std::string segment;
    std::vector<std::string> stringVec;

    for(std::string::const_iterator it(s.begin()); it != s.end(); it++) {
        if(*it == delimiter){
            if(!segment.empty()){
                stringVec.push_back(segment);
                segment.clear();
            }
        }
        else{
            segment += *it;
        }
    }

    if (!segment.empty()){
            stringVec.push_back(segment);
            segment.clear();
    }

    return stringVec;
}
