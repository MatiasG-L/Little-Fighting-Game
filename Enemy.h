#include "raylib.h"
class Enemy{
    public:
    Vector2 position;
    int width;
    int height;
    int maxHealth;
    int health;
    int power;
    int value;
    Texture2D sprite;
    char weakness;
    
    Enemy(Vector2 position, int width, int height, int maxHealth, int health, int power, int value, Texture2D sprite, char weakness){
        this->position = position;
        this->width = width;
        this->height = height;
        this->maxHealth = maxHealth;
        this->health = health;
        this->power = power;
        this->value = value;
        this->sprite = sprite;
        this->weakness = weakness;
    }
};