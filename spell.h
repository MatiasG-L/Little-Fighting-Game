#include "raylib.h"
#pragma once

class Spell{
    public:
    char spellType;
    int potency;
    int manaConsumption;
    char SPfactor;
    std::string name;
    char attackType;
    int speed;
    int range;
    std::string image;
    Texture2D texture;
    Spell(char spellType, int potency, int manaConsumption, char SPfactor, std::string name, char attackType, int speed, int range){
        this->spellType = spellType;
        this->potency = potency;
        this->manaConsumption = manaConsumption;
        this->name = name;
        this->speed = speed;
        this->range = range;
        this->SPfactor = SPfactor;
        this->attackType = attackType;
    }
     Spell(char spellType, int potency, int manaConsumption, char SPfactor, std::string name, char attackType, int speed, int range, std::string image){
        this->spellType = spellType;
        this->potency = potency;
        this->manaConsumption = manaConsumption;
        this->name = name;
        this->speed = speed;
        this->range = range;
        this->SPfactor = SPfactor;
        this->attackType = attackType;
        this->image = image;
        texture = LoadTexture(image.c_str());
    }
    
};