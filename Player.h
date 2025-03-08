#include "raylib.h"
#include <iostream>


    //---------------------------------------------------------------------------//
   //  NOTE:                                                                    //            
  //      the players origin is at the top left corner of the drawn square     //
 //                                                                           // 
//--------------------------------------------------------------------------//


class Player{
    
    public:
    // player attributes *scale together to maintan the apperance of the player square*
    float width = 80;
    float height = 160;
    Rectangle Rec; //Rectangle struct used for the DrawRectanglePro function
    
    int maxHealth;
    int health;
    
    int maxMana;
    int mana;
    
    int maxStamina;
    int stamina;
    
    int level = 1;
    int EXP = 0;
    int nextLevel = 200; 
    
    typedef struct Stats{
        int mana;
        int endurence;
        int agility;
        int skill;
        int power;
        int vitality;
    }Stats;
    
    Stats stats;
    // player state
    Vector2 position;// struct contaning the players x and y coordinate values
   
    //non paramaterized constructer of the player class
    Player(){
        //initilization of some instance variables
        position.x = 600;
        position.y = 400;
        //updates the Rec with the players newly set variable values
        Rec = {position.x, position.y, width, height};
        
        stats.mana = 1;
        stats.endurence = 1;
        stats.agility = 1;
        stats.skill = 1;
        stats.power = 1;
        stats.vitality = 1;
        
        maxHealth = 100 + (stats.vitality * 10);
        health = maxHealth;
        maxMana = 100 + (stats.mana * 10);
        mana = maxMana;
        maxStamina = 100 + (stats.endurence * 10);
        stamina = maxStamina;
    }
    
    void updateHealth(int amount){
        float update;
        if(amount < 0) update = (amount*-1) - (((float)stats.vitality/200)*(amount*-1));
        else update = -amount;
        health -= update;
        if(health > maxHealth) health = maxHealth;
        if(health < 0) health = 0;
    }
    void updateMana(int amount){
        float update;
        if(amount < 0) update = (amount*-1) - (((float)stats.mana/105)*(amount*-1));
        else update = -amount;
        mana -= update;
        if(mana > maxMana) mana = maxMana;
        if(mana < 0) mana = 0;
    }
    
    void updateStat(int amount, char stat){
        if(stat == 'm'){
            stats.mana += amount;
            maxMana = 100 + (stats.mana * 10);
            if(stats.mana > 100) stats.mana = 100;
        }else if(stat == 'e'){
            stats.endurence += amount;
            maxStamina = 100 + (stats.endurence * 10);
            if(stats.endurence > 100) stats.endurence = 100;
        }else if(stat == 'a'){
            stats.agility += amount;
            if(stats.agility > 100) stats.agility = 100;
        }else if(stat == 's'){
            stats.skill += amount;
            if(stats.skill > 100) stats.skill = 100;
        }else if(stat == 'p'){
            stats.power += amount;
            if(stats.power > 100) stats.power = 100;
        }else if(stat == 'v'){
            stats.vitality += amount;
            maxHealth = 100 + (stats.vitality * 10);
            if(stats.vitality > 100) stats.vitality = 100;
        }
        
    }
    
    void exp(int amount){
        if(EXP > nextLevel){
            EXP -= nextLevel;
            level += 1;
            nextLevel *= 1.5;
            updateStat(1, 'm');
            updateStat(1, 'e');
            updateStat(1, 'a');
            updateStat(1, 's');
            updateStat(1, 'p');
            updateStat(1, 'v');
        }else EXP += amount;
        
    }
};