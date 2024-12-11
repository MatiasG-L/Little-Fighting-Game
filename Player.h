#include "raylib.h"


    //---------------------------------------------------------------------------//
   //  NOTE:                                                                    //            
  //      the players origin is at the top left corner of the drawn square     //
 //                                                                           // 
//---------------------------------------------------------------------------//


class Player{
    
    public:
    // player attributes *scale together to maintan the apperance of the player square*
    float width = 50;
    float height = 100;
    Rectangle Rec; //Rectangle struct used for the DrawRectanglePro function
    
    int maxHealth;
    int health;
    
    int maxMana;
    int mana;
    
    int maxStamina;
    int stamina;
    
    typedef struct Stats{
        int mana;
        int stamina;
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
        stats.stamina = 1;
        stats.agility = 1;
        stats.skill = 1;
        stats.power = 1;
        stats.vitality = 1;
        
        maxHealth = 100 + (stats.vitality * 10);
        health = maxHealth;
        maxMana = 100 + (stats.mana * 10);
        mana = maxMana;
        maxStamina = 100 + (stats.stamina * 10);
        stamina = maxStamina;
    }
    
};