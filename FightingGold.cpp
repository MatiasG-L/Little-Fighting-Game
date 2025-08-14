/**************************************************************************************************************
*
*   COOL LITTLE GAME IN C++, USING: RAYLIB 5.0
*
**************************************************************************************************************/



     //---------------------------------------------------------------------------//
    //  NOTE:                                                                    //            
   //     when subtracting from an objects position on the y axis; it will      //
  //      appear to be moving up and vise versa meaning that, up is a          // 
 //       lower value or negative, while down is a positive or higher number  // 
//---------------------------------------------------------------------------//



#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#include "Wall.h"
#include "raylib.h"
#include "Player.h"
#include "spell.h"
#include "Slot.h"
#include "projectile.h"
#include "Enemy.h"
#include "banner.h"

//macro defined literals that represent predefined velocities in 8 direction that take a single paramater for speed
#define NORTH(s)  CLITERAL(Vector2){ 0, -s } 
#define SOUTH(s)  CLITERAL(Vector2){ 0, s } 
#define WEST(s)  CLITERAL(Vector2){ -s, 0 } 
#define EAST(s)  CLITERAL(Vector2){ s, 0 } 
#define SOUTHEAST(s)  CLITERAL(Vector2){ s, s }
#define NORTHEAST(s) CLITERAL(Vector2){ s, -s }
#define NORTHWEST(s)  CLITERAL(Vector2){ -s, -s } 
#define SOUTHWEST(s)  CLITERAL(Vector2){ -s, s } 

//macro that defines the players scale for graphical rendering
#define PLAYER_SCALE 3.5
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

Player player;// creation the player object

//function that returns a Vector2 for new player position given collision and speed
Vector2 movementRequestS(char axis, int amountY, Vector2 position);
//handles spell casting taking a spell as a paramater

void spellCast(Spell spell);

void addSpellBanner(Slot slot);
void addSpellBanner(Spell spell);
void addBanner(std::string text);


char playerHealth[50];
char playerMana[50];
char playerStamina[50];
char playerLevel[50];
char playerEXP[50];
char playerStat[50];

std::vector<Wall> walls;
std::vector<projectile> projectiles;
std::vector<Spell> spellList;
std::vector<Enemy> enemies;
std::vector<banner> itemTemp;

// used to render the level or main menu
bool inventoryUI = false;
bool stat = false;
bool playerANM = false;


typedef struct inventory{
    std::vector<Slot> spells; 
}inventory;

 Spell *currentSpell;
 
 bool healEffect = false;
 float healWidth = 200;


 int indexI = 0;
 int indexH = 0;
 
 int scrollOffset = 275;
 
 bool combat = false; 
 bool turn = true;
 Enemy *target;

Vector2 dragPos;

bool inventoryDragS = false;
bool hotBarDrag = false;
int main(void)
{
   
    
    // Initialization
    //--------------------------------------------------------------------------------------
    //creating variables to set the screen dimentions to
    const int screenWidth = 1600;
    const int screenHeight = 900;
    
    
    
    InitWindow(screenWidth, screenHeight, " |Fight Game| "); //initilisation of the window 

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    
    // a clear teture to make things transperent if needed
    Texture2D Clear = LoadTexture("Sprites/Icons/Clear.png");
    //Texture loading
    Texture2D PlayerTexture = LoadTexture("Sprites/Player/Player.png");
    Texture2D PlayerIdleRight = LoadTexture("Sprites/Player/Player_idle_right.png");
    Texture2D PlayerIdleLeft = LoadTexture("Sprites/Player/Player_idle_left.png");
    Texture2D PlayerWalkRight = LoadTexture("Sprites/Player/Player_walk_right.png");
    Texture2D PlayerWalkLeft = LoadTexture("Sprites/Player/Player_walk_left.png");
    Texture2D PlayerCast = LoadTexture("Sprites/Player/Player_cast.png");
    Texture2D TreeTexture = LoadTexture("Sprites/Objects/Tree.png");
    Texture2D FireBarPhy = LoadTexture("Sprites/Objects/FireBall.png");
    Texture2D Dummy = LoadTexture("Sprites/Objects/Dummy.png");
    Texture2D Circle = LoadTexture("Sprites/Icons/Circle.png");
    Texture2D PlusButton = LoadTexture("Sprites/Icons/PlusButton.png");
    
    PlayerIdleRight.width *= PLAYER_SCALE;
    PlayerIdleRight.height *= PLAYER_SCALE;
    
    PlayerIdleLeft.width *= PLAYER_SCALE;
    PlayerIdleLeft.height *= PLAYER_SCALE;
    
    PlayerWalkRight.width *= PLAYER_SCALE;
    PlayerWalkRight.height *= PLAYER_SCALE;
    
    PlayerWalkLeft.width *= PLAYER_SCALE;
    PlayerWalkLeft.height *= PLAYER_SCALE;
    
    PlayerCast.width *= PLAYER_SCALE;
    PlayerCast.height *= PLAYER_SCALE;
    
    player.walkR = {4, 4, PlayerWalkRight.width, PlayerWalkRight.height, "walkR"};
    player.walkL = {4, 4, PlayerWalkLeft.width, PlayerWalkLeft.height, "walkL"};
    player.idleR = {4, 3, PlayerIdleRight.width, PlayerIdleRight.height, "idle"};
    player.idleL = {4, 3, PlayerIdleLeft.width, PlayerIdleLeft.height, "idle"};
    player.cast = {6, 4, PlayerCast.width, PlayerCast.height, "cast"};
    
    player.changeAnimation("idle", PlayerIdleRight);
    
    Wall wall1(200, 200, 165, 300, TreeTexture);
    Wall wall2(700, 200, 165, 300, TreeTexture);
    Wall wall3(900, 500, 165, 300, TreeTexture);
    Wall wall4(-200, -100, 165, 300, TreeTexture);
    Wall wall5(500, 900, 165, 300, TreeTexture);
    Wall wall6(200, 1200, 165, 300, TreeTexture);

    walls.push_back(wall1);
    walls.push_back(wall2);
    walls.push_back(wall3);
    walls.push_back(wall4);
    walls.push_back(wall5);
    walls.push_back(wall6);

    
    std::cout << "\n(WALLS: " <<  walls.size() << ")\n";
        
    Spell fireBall('a', 20, 15, 'f', "Fire Ball", 'p',/*speed*/ 10, 500, "Sprites/Icons/Flame.png", "Sprites/Objects/FireBall.png",2);// creates a spell
    Spell waterBall('a', 20, 12, 'w', "Water Ball", 'p',/*speed*/ 10, 600, "Sprites/Icons/WaterDrop.png", "Sprites/Objects/WaterBall.png",2.5);// creates a spell
    Spell lightningBolt('a', 25, 25, 'l', "Lightning Bolt", 'b',/*speed*/ 30, 1000, "Sprites/Icons/LightningBolt.png", "Sprites/Objects/LightingBolt.png",4);// creates a spell
    Spell Spike('a', 100, 60, 's', "Earth Spike", 'p',/*speed*/ 22, 1000, "Sprites/Icons/Spike.png", "Sprites/Objects/RockSpike.png",9);// creates a spell
    Spell lightningSpear('a', 150, 90, 'l', "Lightning Spear", 'b',/*speed*/ 40, 1200, "Sprites/Icons/LightningSpear.png", "Sprites/Objects/LightningSpear.png",7.5);// creates a spell
    Spell waterSpear('a', 45, 60, 'w', "Water Spear", 'b',/*speed*/ 25, 900, "Sprites/Icons/WaterSpear.png", "Sprites/Objects/WaterSpear.png",8);// creates a spell
    Spell rock('a', 25, 20, 's', "Rock", 'b',/*speed*/ 5, 700, "Sprites/Icons/Rock.png", "Sprites/Objects/Rock.png", 3);// creates a spell
    Spell fireArrow('a', 160, 95, 'f', "Fire Arrow", 'b',/*speed*/ 15, 700, "Sprites/Icons/FireArrow.png", "Sprites/Objects/FireArrow.png",7);// creates a spell
    Spell weakHeal('h', 10, 6, 'h', "Weak Heal", '0',/*speed*/ 0, 0, "Sprites/Icons/WeakHeal.png", "Sprites/Icons/Clear.png",0);// creates a healing spell
    Spell lightHeal('h', 20, 5, 'h', "Light Heal", '0',/*speed*/ 0, 0, "Sprites/Icons/LightHeal.png", "Sprites/Icons/Clear.png",0);// creates a healing spell
    Spell midHeal('h', 40, 4, 'h', "Mid Heal", '0',/*speed*/ 0, 0, "Sprites/Icons/MidHeal.png", "Sprites/Icons/Clear.png",0);// creates a healing spell
    Spell strongHeal('h', 60, 3, 'h', "Strong Heal", '0',/*speed*/ 0, 0, "Sprites/Icons/StrongHeal.png", "Sprites/Icons/Clear.png",0);// creates a healing spell
    Spell superHeal('h', 100, 2, 'h', "Super Heal", '0',/*speed*/ 0, 0, "Sprites/Icons/SuperHeal.png", "Sprites/Icons/Clear.png",0);// creates a healing spell
    Spell fullHeal('h', 10000, 1, 'h', "Full Heal", '0',/*speed*/ 0, 0, "Sprites/Icons/FullHeal.png", "Sprites/Icons/Clear.png",0);// creates a healing spell
    Spell weakBuffS('b', 0.3, 10, 's', "Weak Strength Buff", '1',/*speed*/ 0, 0, "Sprites/Icons/WeakBuff.png", "Sprites/Icons/Clear.png",0);// creates a buff spell
    Spell weakBuffE('b', 0.3, 10, 'r', "Weak Endurance Buff", '0',/*speed*/ 0, 0, "Sprites/Icons/WeakBuffEnd.png", "Sprites/Icons/Clear.png",0);// creates a buff spell
    addSpellBanner(fireBall);
    addSpellBanner(waterBall);
    addSpellBanner(lightningBolt);
    addSpellBanner(Spike);
    addSpellBanner(lightningSpear);
    addSpellBanner(waterSpear);
    addSpellBanner(rock);
    addSpellBanner(fireArrow);
    addSpellBanner(weakHeal);
    addSpellBanner(lightHeal);
    addSpellBanner(midHeal);
    addSpellBanner(strongHeal);
    addSpellBanner(superHeal);
    addSpellBanner(fullHeal);
    addSpellBanner(weakBuffS);
    addSpellBanner(weakBuffE);
    
    
    Slot slot1({0, 0}, 100, 100, &fireBall);
    Slot slot2({0, 0}, 100, 100, &waterBall);
    Slot slot3({250, 250}, 100, 100, &lightningBolt);
    Slot slot4({250, 250}, 100, 100, &Spike);
    Slot slot5({250, 250}, 100, 100, &lightningSpear);
    Slot slot6({250, 250}, 100, 100, &rock);
    Slot slot7({250, 250}, 100, 100, &waterSpear);
    Slot slot8({250, 250}, 100, 100, &fireArrow);
    
    Slot slot9({250, 250}, 100, 100, &weakHeal);
    Slot slot10({250, 250}, 100, 100, &lightHeal);
    Slot slot11({250, 250}, 100, 100, &midHeal);
    Slot slot12({250, 250}, 100, 100, &strongHeal);
    Slot slot13({250, 250}, 100, 100, &superHeal);
    Slot slot14({250, 250}, 100, 100, &fullHeal);
    
    Slot slot15({250, 250}, 100, 100, &weakBuffS);
    Slot slot16({250, 250}, 100, 100, &weakBuffE);
    
     inventory inventory;// created an instance for the inventory
     struct inventory hotBar;//created an instance for the hotbar
     
     Slot slotH1({250, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH2({450, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH3({650, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH4({850, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH5({1050, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH6({1250, 750}, 100, 100, "Sprites/Icons/Clear.png");
     
     hotBar.spells.push_back(slotH1);
     hotBar.spells.push_back(slotH2);
     hotBar.spells.push_back(slotH3);
     hotBar.spells.push_back(slotH4);
     hotBar.spells.push_back(slotH5);
     hotBar.spells.push_back(slotH6);
     
     inventory.spells.push_back(slot1);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot2);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot3);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot4);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot5);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot6);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot7);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot8);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot9);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot10);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot11);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot12);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot13);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot14);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot15);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot16);// adds the created spell to the spells vector in the inventory
     
    //creating a vector to be used as a list of moves that can be used as given to an enemy
    std::vector<Spell> dummyMoves = {weakHeal, waterBall, Spike};
    //creating the enemy objects
     Enemy dum({300, 700}, 150, 150, 300, 300, 50, GetRandomValue(40,100), Dummy, 'f',250, 60, dummyMoves,0.4);
     Enemy dum1({500, 900}, 150, 150, 300, 300, 50, GetRandomValue(40,100), Dummy, 'f',250, 60,dummyMoves,0.4);
     Enemy dum2({0, 100}, 150, 150, 300, 300, 50, GetRandomValue(40,100), Dummy, 'w',250, 60,dummyMoves,0.4);
     Enemy dum3({(float)GetRandomValue(0,900), (float)GetRandomValue(0,900)}, 150, 150, 300, 300, 50, GetRandomValue(40,120), Dummy, 'w', 250, 60,dummyMoves,0.4);
     Enemy dum4({(float)GetRandomValue(0,900), (float)GetRandomValue(0,900)}, 150, 150, 300, 300, 50, GetRandomValue(40,120), Dummy, 'w', 250, 60,dummyMoves,0.4);
     Enemy dum5({(float)GetRandomValue(0,900), (float)GetRandomValue(0,900)}, 150, 150, 300, 300, 50, GetRandomValue(40,120), Dummy, 'w', 250, 60,dummyMoves,0.4);
     Enemy dum6({(float)GetRandomValue(0,900), (float)GetRandomValue(0,900)}, 150, 150, 300, 300, 50, GetRandomValue(40,120), Dummy, 'w', 250, 60,dummyMoves,0.4);
     Enemy dum7({(float)GetRandomValue(0,900), (float)GetRandomValue(0,900)}, 150, 150, 300, 300, 50, GetRandomValue(40,120), Dummy, 'w', 250, 60,dummyMoves,0.4);
     Enemy dum8({(float)GetRandomValue(0,900), (float)GetRandomValue(0,900)}, 150, 150, 300, 300, 50, GetRandomValue(40,120), Dummy, 'w', 250, 60,dummyMoves,0.4);
     //adding the created enemies to the enemies vector 
     enemies.push_back(dum);
     enemies.push_back(dum1);
     enemies.push_back(dum2);
     enemies.push_back(dum3);
     enemies.push_back(dum4);
     enemies.push_back(dum5);
     enemies.push_back(dum6);
     enemies.push_back(dum7);
     enemies.push_back(dum8);

    for(int i = 0; i<inventory.spells.size(); i++){
        inventory.spells[i].spell->texture.width = 100;
        inventory.spells[i].spell->texture.height = 100;
    }
    
    PlayerTexture.width = 160;
    PlayerTexture.height = 210;
    
    PlusButton.width = 30;
    PlusButton.height = 30;
    
    
    for(int i = 0; i < walls.size(); i++){
        walls[i].texture.width = 500;
        walls[i].texture.height = 500;
    }
    
    //initializes camera values
    Camera2D camera = { 0 };
    camera.offset = {screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.target = {0,0};

    //limits fps for more univarsal experience
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    float timerWalk = 0;
    float timerRegen = 0;
    float timerRegenMana = 0;
    float timerCamera = 0;
    float spellCastTimer = 0;
    std::cout << "\n" << player.health << "\n";
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        player.animation();
        
        if(timerRegenMana < 0.01){
            timerRegenMana += GetFrameTime();
        }else if(player.mana < player.maxMana && !combat){
            player.mana += 1;
            timerRegenMana = 0;
        }
        
        for(int i = 0; i < enemies.size(); i++){
            if(distance(enemies[i].center(), player.center()) <= enemies[i].range && target == NULL){
                target = &enemies[i];
                combat = true;
            }
        }
        
        //projectile collision
        for(size_t i = projectiles.size(); i-- > 0;){
            for(size_t j = enemies.size(); j-- > 0;){
                if(CheckCollisionRecs({projectiles[i].position.x, projectiles[i].position.y, projectiles[i].width, projectiles[i].height},{enemies[j].position.x, enemies[j].position.y, enemies[j].width, enemies[j].height}) && projectiles[i].freindly){      
                    enemies[j].position.x += ((enemies[j].position.x - midWayPoint(enemies[j].position, player.position).x) * 0.1) * projectiles[i].knockback;
                    enemies[j].position.y += ((enemies[j].position.y - midWayPoint(enemies[j].position, player.position).y) * 0.1) * projectiles[i].knockback;
                    enemies[j].damage((projectiles[i].power * (1 + (float)player.stats.power * 0.1) *player.damgMultiplier), projectiles[i].SPfactor);
                    //projectiles.erase(projectiles.begin() + i);
                    std::swap(projectiles[i], projectiles.back());
                    projectiles.pop_back();
                    
                    
                }else if(CheckCollisionRecs({projectiles[i].position.x, projectiles[i].position.y, projectiles[i].width, projectiles[i].height},{player.position.x, player.position.y, player.width, player.height}) && !projectiles[i].freindly){
                    player.position.x += ((player.position.x - midWayPoint(player.position, enemies[j].position).x) * 0.1) * projectiles[i].knockback;
                    player.position.y += ((player.position.y - midWayPoint(player.position, enemies[j].position).y) * 0.1) * projectiles[i].knockback;
                    player.updateHealth(-projectiles[i].power);
                    //projectiles.erase(projectiles.begin() + i);
                    std::swap(projectiles[i], projectiles.back());
                    projectiles.pop_back();
                    
                    
                    
                }
               
                if(enemies[j].health <= 0){
                    player.exp(enemies[j].value);
                    target = NULL; 
                    combat = false;
                    std::cout << "\n" << enemies[j].value << "\n";
                    //enemies.erase(enemies.begin() + j);
                    std::swap(enemies[j], enemies.back());
                    enemies.pop_back();
                    
                    
                    
                }
            }
        }
       
       if(player.lvlnot > 0){
           for(int i = 0; i < player.lvlnot; i++){
                addBanner("Player : Leveled Up!");
           }
           player.lvlnot = 0;
       }
       
       if(IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A)){           
                if( !player.flipped){
                    player.changeAnimation("walkR", PlayerWalkRight);
                    
                }else if(player.flipped){
                    player.changeAnimation("walkL", PlayerWalkLeft);
                }
                
       }else{
           if(!player.flipped){
                player.changeAnimation("idle", PlayerIdleRight);
           }else{
               player.changeAnimation("idle", PlayerIdleLeft);
           }
       }
       
       
       
       //stamina regeneration
        if(timerRegen < 0.05){
            timerRegen += GetFrameTime();
        }else if(player.stamina < player.maxStamina && !IsKeyDown(KEY_LEFT_SHIFT) && !combat){
            player.stamina += 2;
            timerRegen = 0;
        }
         //player movement when holding down the keys
        if(timerWalk < 0.05){
           timerWalk += GetFrameTime();
        }else if(!combat){
            if(IsKeyDown(KEY_D) && (!IsKeyDown(KEY_LEFT_SHIFT) || player.stamina <= 0)){
                player.position = movementRequestS('x', player.stats.agility  * 2 + 5, player.position);
                player.flipped = false;
                
            }else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                player.position = movementRequestS('x', player.stats.agility  * 2 + 10, player.position);
                player.stamina -= 2;
                timerRegen = 0;
            }
            if(IsKeyDown(KEY_A) && (!IsKeyDown(KEY_LEFT_SHIFT) || player.stamina <= 0)){
                player.position = movementRequestS('x', -player.stats.agility  * 2 - 5, player.position);
                player.flipped = true;
            }else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                player.position = movementRequestS('x', -player.stats.agility  * 2 - 10, player.position);
                player.stamina -= 2;
                timerRegen = 0;
            }
            if(IsKeyDown(KEY_W) && (!IsKeyDown(KEY_LEFT_SHIFT) || player.stamina <= 0)){
                player.position = movementRequestS('y', -player.stats.agility  * 2 - 5, player.position);
                
            }else if(IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                player.position = movementRequestS('y', -player.stats.agility  * 2 - 10, player.position);
                player.stamina -= 2;
                timerRegen = 0;
            }
            if(IsKeyDown(KEY_S) && (!IsKeyDown(KEY_LEFT_SHIFT) || player.stamina <= 0)){
                player.position = movementRequestS('y', player.stats.agility  * 2 + 5, player.position);
                
            }else if(IsKeyDown(KEY_S) && IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                player.position = movementRequestS('y', player.stats.agility  * 2 + 10, player.position);
                player.stamina -= 2;
                timerRegen = 0;
            }
            
            timerWalk = 0;
       }
       //playermovement when pressing the keys
       
       if(IsKeyPressed(KEY_UP) && inventoryUI && inventory.spells[inventory.spells.size()-1].position.y > 276){
           scrollOffset -= 150;
       }else if(IsKeyPressed(KEY_DOWN) && inventoryUI && inventory.spells[0].position.y < 274){
           scrollOffset += 150;
       }else if(!inventoryUI) scrollOffset = 275;
       
       if(!combat){
        if(IsKeyPressed(KEY_D)){
            player.position = movementRequestS('x', player.stats.agility  * 2 +  5, player.position);
        }
        if(IsKeyPressed(KEY_A)){
            player.position = movementRequestS('x', -player.stats.agility  * 2 - 5, player.position);
        }
        if(IsKeyPressed(KEY_W)){
            player.position = movementRequestS('y', -player.stats.agility  * 2 - 5, player.position);
        }
        if(IsKeyPressed(KEY_S)){
            player.position = movementRequestS('y', player.stats.agility  * 2 +  5, player.position);
        }
       }else if(player.stamina >= 10){
            if(IsKeyPressed(KEY_D)){
                player.position = movementRequestS('x', player.stats.agility  * 2 +  15, player.position);
                player.stamina -= 10;
            }
            if(IsKeyPressed(KEY_A)){
                player.position = movementRequestS('x', -player.stats.agility  * 2 - 15, player.position);
                player.stamina -= 10;
            }
            if(IsKeyPressed(KEY_W)){
                player.position = movementRequestS('y', -player.stats.agility  * 2 - 15, player.position);
                player.stamina -= 10;
            }
            if(IsKeyPressed(KEY_S)){
                player.position = movementRequestS('y', player.stats.agility  * 2 + 15, player.position);
                player.stamina -= 10;
            }
       }
       
       if(!turn && !target->turn){
           spellList = target->moveList(player.health, player.position);
           target->mana  = target->maxMana;
           target->turn = true;
       }
       if(!turn && target->turn && spellList.size() == 0){
           turn = true;
           target->turn = false;
       }
       if(spellCastTimer < 0.5){
            spellCastTimer += GetFrameTime();
            
        }else{
            std::cout << "\n" << spellList.size() << "\n";
            if(spellList.size() > 0){
                projectile ball(target->center(), spellList[spellList.size()-1].range, NORTHWEST(spellList[spellList.size()-1].speed), spellList[spellList.size()-1].potency * target->DamageScale, spellList[spellList.size()-1].SPfactor, 60, 60, spellList[spellList.size()-1].shoot , 0,spellList[spellList.size()-1].speed ,spellList[spellList.size()-1].knockback, false);
                projectiles.push_back(ball);
                spellList.pop_back();
            }
            spellCastTimer = 0;
        }
       
       
       
       
       if(combat && player.center().x < target->center().x){
           player.flipped = false;
       }
       if(combat && player.center().x > target->center().x){
           player.flipped = true;
       }
       
       if(target == NULL){
          camera.target = {lerp(camera.target.x, player.position.x, 0.7 * GetFrameTime()), lerp(camera.target.y, player.position.y, 0.7 * GetFrameTime())}; 
       }else{
           camera.target = {lerp(camera.target.x, lerp(player.position.x, target->position.x, 0.5), 0.9 * GetFrameTime()), lerp(camera.target.y, lerp(player.position.y, target->position.y, 0.5), 0.9 * GetFrameTime())}; 
       }
       
            
        if(IsKeyPressed(KEY_R)){
            target = NULL; 
            combat = false;
        }
        if(IsKeyPressed(KEY_H)){
            player.updateHealth(10);
        }
        if(IsKeyPressed(KEY_E) && !combat){
            inventoryUI = !inventoryUI;
        }
        if(IsKeyPressed(KEY_G)){
            inventory.spells.push_back(slot3);
            addSpellBanner(slot3);
        }
        if(IsKeyPressed(KEY_P)){
            player.updateStat(2,'m');
            player.updateStat(2,'e');
            player.updateStat(2,'a');
            player.updateStat(2,'s');
            player.updateStat(2,'p');
            player.updateStat(2,'v');
        }
        
    
        if(turn && combat){
            if(IsKeyPressed(KEY_ONE) && hotBar.spells[0].spell != NULL){
                currentSpell = hotBar.spells[0].spell;
                spellCast(*currentSpell);
            }
             if(IsKeyPressed(KEY_TWO) && hotBar.spells[1].spell != NULL){
                currentSpell = hotBar.spells[1].spell;
                spellCast(*currentSpell);
            }
             if(IsKeyPressed(KEY_THREE) && hotBar.spells[2].spell != NULL){
                currentSpell = hotBar.spells[2].spell;
                spellCast(*currentSpell);
            }
             if(IsKeyPressed(KEY_FOUR) && hotBar.spells[3].spell != NULL){
                currentSpell = hotBar.spells[3].spell;
                spellCast(*currentSpell);
            }
             if(IsKeyPressed(KEY_FIVE) && hotBar.spells[4].spell != NULL){
                currentSpell = hotBar.spells[4].spell;
                spellCast(*currentSpell);
            }
             if(IsKeyPressed(KEY_SIX) && hotBar.spells[5].spell != NULL){
                currentSpell = hotBar.spells[5].spell;
                spellCast(*currentSpell);
            }
        }
        
        if(IsKeyPressed(KEY_V)){
            player.exp(70);
        }
        if(IsKeyPressed(KEY_N)){
            player.exp(10000);
        }
        if(IsKeyPressed(KEY_E) && combat){
            player.mana = player.maxMana;
            player.stamina = player.maxStamina;
            turn = !turn;
            std::cout<<"\n" << player.damgMultiplier << "\n";
            if(player.persistance){ 
                player.persistance = false;
            }else{
                player.damgMultiplier = 1;
                player.resistanceMultiplier = 1;
            }
        }
        if(!combat){
            player.damgMultiplier = 1;
            player.resistanceMultiplier = 1;
        }
        
        for(int i = 0; i<projectiles.size(); i++){
            if(CheckCollisionPointCircle({projectiles[i].position.x, projectiles[i].position.y}, projectiles[i].start, projectiles[i].range) && target != NULL){
                if(projectiles[i].freindly){
                    projectiles[i].position = lerpV(projectiles[i].position, target->center(), projectiles[i].speed*0.01);
                    if(target != NULL)projectiles[i].rotation = (float)(atan2((double)target->center().y - (double)player.center().y, (double)target->center().x - (double)player.center().x)*(180/PI));
                }
                else{
                    projectiles[i].position = lerpV(projectiles[i].position, player.center(), projectiles[i].speed*0.01);
                    if(target != NULL)projectiles[i].rotation = (float)(atan2((double)player.center().y - (double)target->center().y, (double)player.center().x - (double)target->center().x)*(180/PI));
                }
            }else{
                projectiles.erase(projectiles.begin() + i);
            }
        }
      // Draw, where the scene actually gets rendered and drawn out

      
        BeginDrawing();
        
        
            
            //anything drawn inside of the BeginMode2D() and EndMode2D() are going to be drawn onto the world and wont move with the camera but anything drawn after EndMode2D() is drawn onto the screen and moves with the camera useful for UI
            BeginMode2D(camera);
            
                
            
                ClearBackground(DARKGREEN);
                
                //draws the player
                
                
                if(healEffect){
                    healWidth = lerp(healWidth, 0, 15 * GetFrameTime());
                    DrawRectangle(player.position.x + player.width/2 + healWidth, player.position.y - 1000, healWidth, 2000 ,GREEN);
                    DrawRectangle(player.position.x + player.width/2 - healWidth, player.position.y - 1000, healWidth, 2000 ,GREEN);
                    DrawCircle(player.position.x + player.width/2, player.position.y, healWidth, GREEN);
                }
                if(healWidth < 1){
                    healWidth = 200;
                    healEffect = false;
                }
                
               // DrawTexture(PlayerTexture, player.position.x - 40, player.position.y-45, WHITE);
               // DrawTextureRec(player.texture, player.animRec, vectorSubtraction(&player.position, new Vector2{45,45}), WHITE);
                
                //draws the walls & enemies
                bool playerL = false;
                    for(int i = 0; i < walls.size(); i++){

                        if(player.position.y < walls[i].position.y +120 && !playerL){
                            DrawTextureRec(player.texture, player.animRec, vectorSubtraction(&player.position, new Vector2{45,120}), WHITE);
                            playerL = true;
                        }

                        DrawTexture(walls[i].texture, walls[i].position.x-190, walls[i].position.y - 185, WHITE);
                        //DrawRectangleLines(walls[i].position.x, walls[i].position.y, walls[i].width, walls[i].height, WHITE);
                    }
                if(!playerL){
                    DrawTextureRec(player.texture, player.animRec, vectorSubtraction(&player.position, new Vector2{45,120}), WHITE);
                }
                //DrawRectangleLines(player.position.x, player.position.y, player.width, player.height, BLACK);
                //draws the enemies
                for(int i = 0; i < enemies.size(); ++i){
                    if(&enemies[i] == target){
                    DrawRectangle(enemies[i].position.x, enemies[i].position.y - 30, (((float)enemies[i].health/(float)enemies[i].maxHealth)*enemies[i].width), 20, MAROON);
                    }
                    DrawTextureEx(enemies[i].sprite, {enemies[i].position.x, enemies[i].position.y}, 0, 9.5, WHITE);
                   // DrawRectangleLines(enemies[i].position.x, enemies[i].position.y, enemies[i].width, enemies[i].height, BLACK);
                }
            
                for(int i = 0; i < projectiles.size(); ++i){
                    //DrawTexture(projectiles[i].image, projectiles[i].position.x, projectiles[i].position.y, WHITE);
                    if(projectiles[i].rotation == 90){
                        DrawTextureEx(projectiles[i].image, {projectiles[i].position.x + projectiles[i].width, projectiles[i].position.y}, projectiles[i].rotation, 6, WHITE);
                    }else if(projectiles[i].rotation == 180){
                        DrawTextureEx(projectiles[i].image, {projectiles[i].position.x + projectiles[i].width, projectiles[i].position.y + projectiles[i].height}, projectiles[i].rotation, 6, WHITE);
                    }else if(projectiles[i].rotation == 270){
                        DrawTextureEx(projectiles[i].image, {projectiles[i].position.x, projectiles[i].position.y + projectiles[i].height}, projectiles[i].rotation, 6, WHITE);
                    }else{
                         DrawTextureEx(projectiles[i].image, {projectiles[i].position.x, projectiles[i].position.y}, projectiles[i].rotation, 6, WHITE);
                    }
                   
                   // DrawRectangleLines(projectiles[i].position.x, projectiles[i].position.y, projectiles[i].width, projectiles[i].height, BLACK);
                }
           EndMode2D();
           
           DrawFPS(0,0);
           
           sprintf(playerHealth, "%d / %d", player.health, player.maxHealth);
           sprintf(playerMana, "%d / %d", player.mana, player.maxMana);
           sprintf(playerStamina, "%d / %d", player.stamina, player.maxStamina);
           //draws health bar
           DrawRectangle(50, 50, lerp(500, 1500, (float)player.stats.vitality/100), 40, GRAY);
           DrawRectangle(50, 50, (((float)player.health/(float)player.maxHealth) * lerp(500, 1500, (float)player.stats.vitality/100)), 40, MAROON);
           DrawRectangleLinesEx({50, 50, lerp(500, 1500, (float)player.stats.vitality/100), 40}, 2,BLACK);
           DrawText(playerHealth, 60, 60, 20, BLACK);
           //draws mana bar
           DrawRectangle(50, 100, lerp(500, 1500, (float)player.stats.mana/100), 40, GRAY);
           DrawRectangle(50, 100, (((float)player.mana/(float)player.maxMana) * lerp(500, 1500, (float)player.stats.mana/100)), 40, DARKBLUE);
           DrawRectangleLinesEx({50, 100, lerp(500, 1500, (float)player.stats.mana/100), 40}, 2,BLACK);
           DrawText(playerMana, 60, 110, 20, BLACK);
           //draws stamina bar
           DrawRectangle(50, 150, lerp(500, 1500, (float)player.stats.endurence/100), 40, GRAY);
           DrawRectangle(50, 150, (((float)player.stamina/(float)player.maxStamina) * lerp(500, 1500, (float)player.stats.endurence/100)), 40, DARKGREEN);
           DrawRectangleLinesEx({50, 150, lerp(500, 1500, (float)player.stats.endurence/100), 40}, 2,BLACK);
           DrawText(playerStamina, 60, 160, 20, BLACK);
           if(turn && combat){
               DrawRectangleLinesEx({50, 200, 130, 40}, 2, BLACK);
               DrawText("Your Turn", 60, 210, 20, BLACK);
           }else if(combat){
               DrawRectangleLinesEx({50, 200, 180, 40}, 2, BLACK);
               DrawText("Oponents Turn", 60, 210, 20, BLACK);
           }
           
           
           if(itemTemp.size() > 0){
               for(int i = 0; i < itemTemp.size(); i++){  
                   char textChar[50];
                   sprintf(textChar, "%s", itemTemp[i].text.c_str());
                   DrawRectangle(50, 750 - (i*150), itemTemp[i].width, itemTemp[i].height, CLEARBASE(GRAY,100));
                   DrawText(textChar, 100, 800 - (i*150), 20, WHITE);
               }
               itemTemp.front().decayTimer += GetFrameTime();
               if(itemTemp.front().decayTimer >= itemTemp.front().decayTime){
                   itemTemp.erase(itemTemp.begin());
               }
               
           }
           
           //inventory drawing and draging  
           if(inventoryUI){
               //big rectangle for the backdrop used for opening menu
                DrawRectangle(225, 225, 1200, 500, GRAY);
                //used to toggle to the inventory menu
                DrawRectangle(225, 170, 200, 50, GRAY);
                DrawText("inventory", 250 ,180, 30, BLACK);
                //used to toggle to the stats menu
                DrawRectangle(435, 170, 200, 50, GRAY);
                DrawText("stats", 490 ,180, 30, BLACK);
                
                if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {435, 170, 200, 50}) && IsMouseButtonPressed(0)) stat = true;
                if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {225, 170, 200, 50}) && IsMouseButtonPressed(0)) stat = false;
                
                
                if(!stat){
                
                int posX = 260;
                int posY = scrollOffset;
                
                
                for(int i =0; i < inventory.spells.size(); ++i){
                    
                    
                    if(IsMouseButtonPressed(0) && CheckCollisionPointRec({GetMouseX(), GetMouseY()},{inventory.spells[i].position.x, inventory.spells[i].position.y, inventory.spells[i].width, inventory.spells[i].height}) && (inventory.spells[i].position.y < 576 && inventory.spells[i].position.y > 274)){
                        inventoryDragS = true;
                        indexI = i;
                    }
                   
                    
                    if(!inventoryDragS){
                        inventory.spells[i].position.x = posX;
                        inventory.spells[i].position.y = posY;
                    }
                    
                    if((inventory.spells[i].position.y < 576 && inventory.spells[i].position.y > 274) || (i == indexI && inventoryDragS)){
                        DrawTexture(inventory.spells[i].spell->texture, inventory.spells[i].position.x, inventory.spells[i].position.y, WHITE);
                        DrawRectangleLinesEx({inventory.spells[i].position.x, inventory.spells[i].position.y, 100, 100}, inventory.spells[i].thickness, BLACK); 
                    }
                    
                    
                    
                    posX += 150;
                    if (posX > 250+(150*8)){
                        posX = 260;
                        posY += 150;
                    }
                   if(IsMouseButtonReleased(0) && inventoryDragS){
                   
                       if(CheckCollisionPointRec({GetMouseX(),GetMouseY()},{inventory.spells[i].position.x,inventory.spells[i].position.y,inventory.spells[i].width,inventory.spells[i].height})){
                           Slot temp = inventory.spells[i];
                           inventory.spells[i] = inventory.spells[indexI];
                           inventory.spells[indexI] = temp;
                       }
               } 
                }    
               if(inventoryDragS){
                      inventory.spells[indexI].position.x += GetMouseDelta().x;
                      inventory.spells[indexI].position.y += GetMouseDelta().y;
               }
               
               if(IsMouseButtonReleased(0) && inventoryDragS){
                   for(int i = 0; i < hotBar.spells.size(); ++i){
                       if(CheckCollisionPointRec({GetMouseX(),GetMouseY()},{hotBar.spells[i].position.x,hotBar.spells[i].position.y,hotBar.spells[i].width,hotBar.spells[i].height})){
                           if(hotBar.spells[i].spell == NULL){
                               hotBar.spells[i].addSpell(inventory.spells[indexI].spell);
                               inventory.spells.erase(inventory.spells.begin() + indexI);
                           }else{
                               Spell *temp = hotBar.spells[i].spell;
                               hotBar.spells[i].addSpell(inventory.spells[indexI].spell);
                               inventory.spells[indexI].addSpell(temp);
                               temp = NULL;
                           }
                           
                       }
                   }
               }
                if(IsMouseButtonReleased(0) && inventoryDragS){
                        inventoryDragS = false;
                }
                
           }else{
               
               DrawText("Mana: ", 275, 275, 30, BLACK);
               DrawRectangle(380, 275, 200, 30, DARKGRAY);
               DrawRectangle(380, 275, (((float)player.stats.mana)/100) * 200, 30, DARKGREEN);
               DrawRectangleLinesEx({380, 275, 200, 30}, 2,BLACK);
               if(player.statPoint >= 1 && player.stats.mana<100){
                   DrawTexture(PlusButton, 600, 275, WHITE);
                   if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {600, 275, 30, 30}) && IsMouseButtonPressed(0)){
                        player.updateStat(1, 'm');
                        player.statPoint -= 1; 
                   }
               }else{
                   DrawTexture(PlusButton, 600, 275, SEMICLEAR);
               }
               
               
               
               DrawText("Endurance: ", 275, 325, 30, BLACK);
               DrawRectangle(470, 325, 200, 30, DARKGRAY);
               DrawRectangle(470, 325, (((float)player.stats.endurence)/100) * 200, 30, DARKGREEN);
               DrawRectangleLinesEx({470, 325, 200, 30}, 2,BLACK);
               if(player.statPoint >= 1 && player.stats.endurence<100){
                   DrawTexture(PlusButton, 690, 325, WHITE);
                   if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {690, 325, 30, 30}) && IsMouseButtonPressed(0)){
                        player.updateStat(1, 'e');
                        player.statPoint -= 1; 
                   }
               }else{
                   DrawTexture(PlusButton, 690, 325, SEMICLEAR);
               }
               
               
               DrawText("Agility: ", 275, 375, 30, BLACK);
               DrawRectangle(400, 375, 200, 30, DARKGRAY);
               DrawRectangle(400, 375, (((float)player.stats.agility)/100) * 200, 30, DARKGREEN);
               DrawRectangleLinesEx({400, 375, 200, 30}, 2,BLACK);
                if(player.statPoint >= 1&& player.stats.agility<100){
                   DrawTexture(PlusButton, 610, 375, WHITE);
                   if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {610, 375, 30, 30}) && IsMouseButtonPressed(0) ){
                        player.updateStat(1, 'a');
                        player.statPoint -= 1; 
                   }
               }else{
                   DrawTexture(PlusButton, 610, 375, SEMICLEAR);
               }
               
               
               DrawText("Skill: ", 275, 425, 30, BLACK);
               DrawRectangle(350, 425, 200, 30, DARKGRAY);
               DrawRectangle(350, 425, (((float)player.stats.skill)/100) * 200, 30, DARKGREEN);
               DrawRectangleLinesEx({350, 425, 200, 30}, 2,BLACK);
               if(player.statPoint >= 1&& player.stats.skill<100){
                   DrawTexture(PlusButton, 570, 425, WHITE);
                   if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {570, 425, 30, 30}) && IsMouseButtonPressed(0) ){
                        player.updateStat(1, 's');
                        player.statPoint -= 1; 
                   }
               }else{
                   DrawTexture(PlusButton, 570, 425, SEMICLEAR);
               }
               
               
               DrawText("Power: ", 275, 475, 30, BLACK);
               DrawRectangle(390, 475, 200, 30, DARKGRAY);
               DrawRectangle(390, 475, (((float)player.stats.power)/100) * 200, 30, DARKGREEN);
               DrawRectangleLinesEx({390, 475, 200, 30}, 2,BLACK);
               if(player.statPoint >= 1&& player.stats.power<100){
                   DrawTexture(PlusButton, 600, 475, WHITE);
                   if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {600, 475, 30, 30}) && IsMouseButtonPressed(0) ){
                        player.updateStat(1, 'p');
                        player.statPoint -= 1; 
                    }
               }else{
                   DrawTexture(PlusButton, 600, 475, SEMICLEAR);
               }
               
               
               DrawText("Vitality: ", 275, 525, 30, BLACK);
               DrawRectangle(400, 525, 200, 30, DARKGRAY);
               DrawRectangle(400, 525, (((float)player.stats.vitality)/100) * 200, 30, DARKGREEN);
               DrawRectangleLinesEx({400, 525, 200, 30}, 2,BLACK);
               if(player.statPoint >= 1 && player.stats.vitality<100){
                   DrawTexture(PlusButton, 630, 525, WHITE);
                   if(CheckCollisionPointRec({GetMouseX(), GetMouseY()}, {630, 525, 30, 30}) && IsMouseButtonPressed(0) ){
                        player.updateStat(1, 'v');
                        player.statPoint -= 1; 
                    }
               }else{
                   DrawTexture(PlusButton, 630, 525, SEMICLEAR);
               }
               
               sprintf(playerLevel, "Player Level: %d / %d", player.level, 100);
               sprintf(playerEXP, "Next level at: %d / %d", player.EXP, player.nextLevel);
               sprintf(playerStat, "Stat points availible: %d", player.statPoint);
               
               DrawText(playerLevel , 275, 600, 30, BLACK);
               
               DrawText(playerStat , 245, 235, 30, BLACK);
               
               DrawText(playerEXP , 275, 650, 30, BLACK);
               
               DrawRectangle(275, 680, 200, 30, DARKGRAY);
               DrawRectangle(275, 680, lerp(0, 200, (float)player.EXP/(float)player.nextLevel), 30, DARKGREEN);
               DrawRectangleLinesEx({275, 680, 200, 30}, 2,BLACK);
               
               
               DrawTextureEx(Circle, {800, 300}, 0, 0.3, WHITE);
               
               DrawText("M", 750 + (Circle.width/2-Circle.width/4)*0.3, 300, 30, BLACK);
               DrawText("E", 850 + (Circle.width-Circle.width/4)*0.3, 300, 30, BLACK);
               DrawText("V", 750, 300 + (Circle.height/2)*0.3 , 30, BLACK);
               DrawText("A", 820 + (Circle.width)*0.3, 300 + (Circle.height/2)*0.3, 30, BLACK);
               DrawText("P", 750 + (Circle.width/4)*0.3, 300 + (Circle.height)*0.3, 30, BLACK);
               DrawText("S", 800 + (Circle.width-Circle.width/4)*0.3 , 300 + (Circle.height)*0.3, 30, BLACK);

               DrawLineEx(lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width/4)*0.3 ,300}, ((float)player.stats.mana/100)), lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width-Circle.width/4)*0.3 , 300}, ((float)player.stats.endurence/100)), 3, BLACK); 
               DrawLineEx(lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width-Circle.width/4)*0.3 , 300}, ((float)player.stats.endurence/100)), lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width)*0.3 , 300 + (Circle.height/2)*0.3 }, ((float)player.stats.agility/100)), 3, BLACK); 
               DrawLineEx(lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width)*0.3 , 300 + (Circle.height/2)*0.3 }, ((float)player.stats.agility/100)), lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width-Circle.width/4)*0.3 , 300 + (Circle.height)*0.3 }, ((float)player.stats.skill/100)), 3, BLACK); 
               DrawLineEx(lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width-Circle.width/4)*0.3 , 300 + (Circle.height)*0.3 }, ((float)player.stats.skill/100)), lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width/4)*0.3, 300 + (Circle.height)*0.3 }, ((float)player.stats.power/100)), 3, BLACK); 
               DrawLineEx(lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width/4)*0.3, 300 + (Circle.height)*0.3 }, ((float)player.stats.power/100)), lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800, 300 + (Circle.height/2)*0.3 }, ((float)player.stats.vitality/100)), 3, BLACK); 
               DrawLineEx(lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800, 300 + (Circle.height/2)*0.3 }, ((float)player.stats.vitality/100)), lerpV({800 + (Circle.width/2 ) * 0.3, 300 + (Circle.height/2)*0.3}, {800 + (Circle.width/4)*0.3 ,300}, ((float)player.stats.mana/100)), 3, BLACK); 
               
              
                
           }
           
           }
           
           if(combat || inventoryUI){
           //spell slots (hotbar) and draging
           for(int i =0; i < hotBar.spells.size(); i++){
                if(!hotBarDrag){
                    
                    dragPos = hotBar.spells[indexH].position;
                }
               if(inventoryUI){
                    if(IsMouseButtonPressed(0) && CheckCollisionPointRec({GetMouseX(),GetMouseY()},{hotBar.spells[i].position.x,hotBar.spells[i].position.y,hotBar.spells[i].width,hotBar.spells[i].height})){
                        indexH = i;
                        dragPos = hotBar.spells[indexH].position;
                        hotBarDrag = true;
                    }
               }
               
             
               
               if(!(hotBarDrag && indexH == i)){ 
                DrawTexture(hotBar.spells[i].texture, hotBar.spells[i].position.x, hotBar.spells[i].position.y, WHITE);
               }
               DrawRectangleLinesEx({hotBar.spells[i].position.x, hotBar.spells[i].position.y, hotBar.spells[i].width, hotBar.spells[i].height}, hotBar.spells[i].thickness,BLACK); 
               
               
               if(IsMouseButtonReleased(0) && hotBarDrag && CheckCollisionPointRec({GetMouseX(),GetMouseY()}, {hotBar.spells[i].position.x, hotBar.spells[i].position.y, hotBar.spells[i].width, hotBar.spells[i].height})){
                  if(hotBar.spells[i].spell == NULL){
                      hotBar.spells[i].addSpell(hotBar.spells[indexH].spell);
                      hotBar.spells[indexH].removeSpell();
                  }else{  
                    Spell *temp = hotBar.spells[i].spell;
                    hotBar.spells[i].addSpell(hotBar.spells[indexH].spell);
                    hotBar.spells[indexH].addSpell(temp);
                    temp = NULL;
                  }  
               }
           }
           DrawText("1", hotBar.spells[0].position.x + hotBar.spells[0].width/2, hotBar.spells[0].position.y + 110, 20, BLACK);
           DrawText("2", hotBar.spells[1].position.x + hotBar.spells[1].width/2, hotBar.spells[1].position.y + 110, 20, BLACK);
           DrawText("3", hotBar.spells[2].position.x + hotBar.spells[2].width/2, hotBar.spells[2].position.y + 110, 20, BLACK);
           DrawText("4", hotBar.spells[3].position.x + hotBar.spells[3].width/2, hotBar.spells[3].position.y + 110, 20, BLACK);
           DrawText("5", hotBar.spells[4].position.x + hotBar.spells[4].width/2, hotBar.spells[4].position.y + 110, 20, BLACK);
           DrawText("6", hotBar.spells[5].position.x + hotBar.spells[5].width/2, hotBar.spells[5].position.y + 110, 20, BLACK);
           if(hotBarDrag){
               dragPos.x += GetMouseDelta().x;
               dragPos.y += GetMouseDelta().y;
               DrawTexture(hotBar.spells[indexH].texture, dragPos.x , dragPos.y, WHITE);
           }
           if(IsMouseButtonReleased(0) && hotBarDrag && CheckCollisionPointRec({GetMouseX(),GetMouseY()}, {225, 225, 1200, 500})){
                inventory.spells.push_back(hotBar.spells[indexH]);
                hotBar.spells[indexH].removeSpell();
                hotBarDrag = false;
           }
           
           if(IsMouseButtonReleased(0) && hotBarDrag){
                hotBarDrag = false;
           }
           }
           
        //ends the drawing phase of the program     
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
     
     

    // De-Initialization
    //--------------------------------------------------------------------------------------
      // Texture unloading
      for(int i = 0; i < inventory.spells.size(); i++){
          UnloadTexture(inventory.spells[i].spell->texture);
      }
      for(int i = 0; i < walls.size(); i++){
        UnloadTexture(walls[i].texture);
    }
      UnloadTexture(PlayerTexture);
      UnloadTexture(TreeTexture);
      UnloadTexture(FireBarPhy);
      UnloadTexture(Circle);      

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


Vector2 movementRequestS(char axis, int amount, Vector2 position){
    Vector2 result = position;
    bool colD = false;
    if(amount < 0 && (axis == 'X' || axis == 'x')){
        player.flipped = true;
    }
    if(amount > 0 && (axis == 'X' || axis == 'x')){
        player.flipped = false;
    }
    for(int i = 0; i < walls.size(); i++){
           //returns a Vector2 value for the players new position given the parameters and collisions (x value)
        if(axis == 'X' || axis == 'x'){
           if(CheckCollisionRecs({position.x+amount, position.y, player.width, player.height}, walls[i].rec)){
                if(position.x < walls[i].position.x + walls[i].width/2){
                     result.x = walls[i].position.x - player.width;
                }else{
                     result.x = walls[i].position.x + walls[i].width;
                }
                //return the new player position with collision detected
                colD = true;
           }
           //returns a Vector2 value for the players new position given the parameters and collisions (y value)
        }else if(axis == 'y' || axis == 'Y'){
            if(CheckCollisionRecs({position.x, position.y+amount, player.width, player.height}, walls[i].rec)){
                
                if(position.y < walls[i].position.y + walls[i].height/2){
                    result.y = walls[i].position.y - player.height;
                }else{
                    result.y = walls[i].position.y + walls[i].height;
                }
                colD = true;
           }
        }
       }
       
       if(colD){
           return result;
       }else if(axis == 'x' || axis == 'X'){
           return {position.x + amount, position.y};
       }else{
           return {position.x, position.y + amount};
       }
       //return statement that does nothing to make the compiler happy
       return {0,0};
}

void spellCast(Spell spell){
    if(spell.spellType == 'h' && (float)player.maxMana/spell.manaConsumption <= player.mana){
               player.updateMana(-(float)player.maxMana/spell.manaConsumption);
               player.updateHealth(spell.potency);
               healEffect = true;
    }else if(spell.spellType == 'a'&& spell.manaConsumption <= player.mana){
        projectile ball(player.center(), spell.range, NORTHWEST(spell.speed), spell.potency, spell.SPfactor, 60, 60, spell.shoot, 0, spell.speed ,spell.knockback);
        if(ball.freindly)player.updateMana(-spell.manaConsumption);
        projectiles.push_back(ball);
    }else if(spell.spellType == 'b' && (float)player.maxMana/spell.manaConsumption <= player.mana){
        if(spell.SPfactor == 's'){
            if(spell.attackType == '1'){
                player.persistance = true;
            }else{
                player.persistance = false;
            }
            player.damgMultiplier += spell.potency;
            player.updateMana(-(float)player.maxMana/spell.manaConsumption);
        }else if(spell.SPfactor == 'r'){
            if(spell.attackType == '1'){
                player.persistance = true;
            }else{
                player.persistance = false;
            }
            player.resistanceMultiplier += spell.potency;
            player.updateMana(-(float)player.maxMana/spell.manaConsumption);
        }
    }
    
    
}

void addSpellBanner(Slot slot){
    std::string temp = "Spell Acquired: " + slot.spell->name;                
    banner ban(450, 125,{0,0},2, temp);
    itemTemp.push_back(ban);
}
void addSpellBanner(Spell spell){
    std::string temp = "Spell Acquired: " + spell.name;                
    banner ban(450, 125,{0,0},2, temp);
    itemTemp.push_back(ban);
}
void addBanner(std::string text){
    banner ban (400, 125, {0,0}, 5, text);
    itemTemp.push_back(ban);
}



