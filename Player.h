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
    float width = 75;
    float height = 150;
    Rectangle Rec; //Rectangle struct used for the DrawRectanglePro function
    
    int maxHealth;
    int health;
    
    int maxMana;
    int mana;
    
    int maxStamina;
    int stamina;
    
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
    
    void updateStat(int amount, char stat){
        if(stat == 'm'){
            stats.mana += 1;
            maxMana = 100 + (stats.mana * 10);
        }else if(stat == 'e'){
            stats.endurence += 1;
            maxStamina = 100 + (stats.endurence * 10);
        }else if(stat == 'a'){
            stats.agility += 1;
        }else if(stat == 's'){
            stats.skill += 1;
        }else if(stat == 'p'){
            stats.power += 1;
        }else if(stat == 'v'){
            stats.vitality += 1;
            maxHealth = 100 + (stats.vitality * 10);
        }
    }
    
};