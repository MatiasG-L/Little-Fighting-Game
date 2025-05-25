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
    float health;
    int power;
    int value;
    Texture2D sprite;
    char weakness;
    float range;
    float mana;
    float maxMana;
    float DamageScale;
    bool turn;
    
    Enemy(Vector2 position, int width, int height, int maxHealth, float health, int power, int value, Texture2D sprite, char weakness, float range, float maxMana, float DamageScale){
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
        this->maxMana =  maxMana;
        this->mana = maxMana;
        this->DamageScale = DamageScale;
    }
    Enemy(Vector2 position, int width, int height, int maxHealth, float health, int power, int value, Texture2D sprite, char weakness, float range, float maxMana, std::vector<Spell> moves, float DamageScale){
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
        this->maxMana =  maxMana;
        this->mana = maxMana;
        this->moves = moves;
        this->DamageScale = DamageScale;
    }
    
    void damage(float amount, char weakness){
    if(this->weakness == weakness) {amount *= 1.5; std::cout << "\n|WEAKNESS|\n";}
        health -= amount;
    }
    
    Vector2 center(){ return {position.x + width/2, position.y + height/2}; } 
    
    std::vector<Spell> moveList(float playerHealth, Vector2 Position){
        std::vector<Spell> result;
        bool turnEnd = true; 
        
        do{
            turnEnd = true; 
            for(int i = 0; i < moves.size(); i++){
                
                if(mana >= moves[i].manaConsumption && moves[i].spellType == 'a'){
                    if(moves[i].range > distance(position, Position)){
                        turnEnd = false;
                        mana -= moves[i].manaConsumption;
                        result.push_back(moves[i]);
                    }
                }
            }
        }
        while(!turnEnd);
            
        
        return result; 
     }
    
};