#ifndef INCLUDED__BIOAPPLICATION__CREATURE__HPP
#define INCLUDED__BIOAPPLICATION__CREATURE__HPP

#include <string>
#include <vector>
#include "utils.hpp"

/**
 *****************************************************************************
 *
 *  This class is used to load creatures from a file
 *
 *****************************************************************************/

class Creature
{
public:
    enum creatureType { plant, carnivore, herbivore};

    static std::vector<Creature>
                        loadCreaturesFromFile(const std::string& pfad);

    const int ID_;
    const std::string name_;
    const int strength_;
    const int velocity_;
    const int life_;
    const std::string attributes_;
    const std::string imagePath_;
    const bool water_;
    const creatureType type_;

private:
    Creature(int ID, std::string name, int strength, int velocity,
             int life, std::string attributes, std::string imagePath, bool water, creatureType type);

    /* throws Creature load Exception if input is not valid */
    static void validateInput(const std::vector<std::string>& input);

    static void validateName(const std::string& name);
    static void validatePositiveInt(const std::string& val);
    static void validateAttributes(const std::string& attr);
    static void validatePath(const std::string& pfad);
};

#endif // INCLUDED__BIOAPPLICATION__CREATURE__HPP
