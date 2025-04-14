#include "raylib.h"
#include "spell.h"
#include <vector>
class Enemy{
    public:
    std::vector<Spell> moves;
    Vector2 position;
    int width;
    int height;
    int maxHealth;
    int health;
    int power;
    int value;
    Texture2D sprite;
    char weakness;
    float range;
    
    Enemy(Vector2 position, int width, int height, int maxHealth, int health, int power, int value, Texture2D sprite, char weakness, float range){
        this->position = position;
        this->width = width;
        this->height = height;
        this->maxHealth = maxHealth;
        this->health = health;
        this->power = power;
        this->value = value;
        this->sprite = sprite;
        this->weakness = weakness;
        this->range = range;
    }
    
    void damage(int amount, char weakness){
    if(this->weakness == weakness) {amount *= 1.5; std::cout << "\nWEAKNESS\n";}
        health -= amount;
    }
    
    Vector2 center(){ return {position.x + width/2, position.y + height/2}; } 
};