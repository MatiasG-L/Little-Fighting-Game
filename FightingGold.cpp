/**************************************************************************************************************
*
*   SQUARED IN REMASTERED IN C++, USING: RAYLIB 5.0
*
*   IM SOOO COOL (MISIRABLE)*i think i misspelled that*
*    
*   Challange: No tutorials or videos, all logic and gameplay must be original and made by me (raylib documentation isnt       *   included) 
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
#include <iostream>
#include <fstream>
#include <sstream>

#include "Wall.h"
#include "raylib.h"
#include "Player.h"
#include "spell.h"
#include "Slot.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

Player player;// creation the player object

bool movementRequest(char axis, int amount, Vector2 position);
Vector2 movementRequestS(char axis, int amountY, Vector2 position);

std::vector<Wall> walls;

// used to render the level or main menu
bool inventoryUI = false;



typedef struct inventory{
    std::vector<Slot> spells; 
}inventory;


 int indexI = 0;
 int indexH = 0;

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
    //player sprite
    Texture2D PlayerTexture = LoadTexture("Sprites/Player/Player.png");
    Texture2D TreeTexture = LoadTexture("Sprites/Objects/Tree.png");
    
    Wall wall1(200, 200, 195, 300, TreeTexture);
    Wall wall2(700, 200, 195, 300, TreeTexture);

    walls.push_back(wall1);
    walls.push_back(wall2);
    
    std::cout << "\n(WALLS: " <<  walls.size() << ")\n";
    

    
    
    Spell fireBall('a', 10, 15, 'f', "Fire Ball", 'p', 20, 30, "Sprites/Icons/Flame.png");// creates a spell
    Spell waterBall('a', 10, 15, 'w', "Water Ball", 'p', 20, 30, "Sprites/Icons/WaterDrop.png");// creates a spell
    Spell lightningBolt('a', 20, 20, 'l', "Lightning Bolt", 'b', 100, 60, "Sprites/Icons/LightningBolt.png");// creates a spell
    
    Slot slot1({0, 0}, 100, 100, &fireBall);
    Slot slot2({0, 0}, 100, 100, &waterBall);
    Slot slot3({250, 250}, 100, 100, &lightningBolt);
     inventory inventory;// created an instance for the inventory
     struct inventory hotBar;//created an instance for the hotbar
     
     Slot slotH1({450, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH2({650, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH3({850, 750}, 100, 100, "Sprites/Icons/Clear.png");
     Slot slotH4({1050, 750}, 100, 100, "Sprites/Icons/Clear.png");
     
     hotBar.spells.push_back(slotH1);
     hotBar.spells.push_back(slotH2);
     hotBar.spells.push_back(slotH3);
     hotBar.spells.push_back(slotH4);
     
     inventory.spells.push_back(slot1);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot2);// adds the created spell to the spells vector in the inventory
     inventory.spells.push_back(slot3);// adds the created spell to the spells vector in the inventory
    
    


    for(int i = 0; i<inventory.spells.size(); i++){
        inventory.spells[i].spell->texture.width = 100;
        inventory.spells[i].spell->texture.height = 100;
    }
    
    PlayerTexture.width = 90;
    PlayerTexture.height = 160;
    
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
    float timerCamera = 0;
    std::cout << "\n" << player.health << "\n";
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        //player movement when holding down the keys
        
        if(timerRegen < 0.1){
            timerRegen += GetFrameTime();
        }else if(player.stamina < player.maxStamina){
            player.stamina += 1;
            timerRegen = 0;
        }
        
        if(timerWalk < 0.1){
            timerWalk += GetFrameTime();
        }else{
            if(IsKeyDown(KEY_D)){
                player.position = movementRequestS('x', player.stats.agility  * 2 + 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('x', player.stats.agility * 2 + 15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            if(IsKeyDown(KEY_A)){
                player.position = movementRequestS('x', -player.stats.agility * 2 - 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('x', -player.stats.agility * 2 - 15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            if(IsKeyDown(KEY_W)){
                player.position = movementRequestS('y', -player.stats.agility * 2 - 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('y', -player.stats.agility * 2 -  15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            if(IsKeyDown(KEY_S)){
                player.position = movementRequestS('y', player.stats.agility  * 2 +  15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('y', player.stats.agility  * 2 +  15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            timerWalk = 0;
        }
       //playermovement when pressing the keys
       if(IsKeyPressed(KEY_D)){
           player.position = movementRequestS('x', player.stats.agility  * 2 +  15, player.position);
       }
       if(IsKeyPressed(KEY_A)){
           player.position = movementRequestS('x', -player.stats.agility  * 2 - 15, player.position);
       }
       if(IsKeyPressed(KEY_W)){
           player.position = movementRequestS('y', -player.stats.agility  * 2 - 15, player.position);
       }
       if(IsKeyPressed(KEY_S)){
           player.position = movementRequestS('y', player.stats.agility  * 2 +  15, player.position);
       }
       
       camera.target = {lerp(camera.target.x, player.position.x, 1.5 * GetFrameTime()), lerp(camera.target.y, player.position.y, 1.5 * GetFrameTime())};
            
        if(IsKeyPressed(KEY_R)){
            player.updateHealth(-10); 
        }
        if(IsKeyPressed(KEY_Q)){
            inventoryUI = !inventoryUI;
        }
        if(IsKeyPressed(KEY_P)){
            player.updateStat(1,'m');
            player.updateStat(1,'e');
            player.updateStat(1,'a');
            player.updateStat(1,'s');
            player.updateStat(1,'p');
            player.updateStat(1,'v');
        }
        
            
      // Draw, where the scene actually gets rendered and drawn out

      
        BeginDrawing();
            
            
           
            
            //anything drawn inside of the BeginMode2D() and EndMode2D() are going to be drawn onto the world and wont move with the camera but anything drawn after EndMode2D() is drawn onto the screen and moves with the camera useful for UI
            BeginMode2D(camera);
            
                ClearBackground(GREEN);
                
                
                //draws the player
                DrawRectangleLines(player.position.x, player.position.y, player.width, player.height, BLACK);
                DrawTexture(PlayerTexture, player.position.x - 10, player.position.y-5, WHITE);
                
                //draws the walls
                for(int i = 0; i < walls.size(); i++){
                    DrawRectangleLines(walls[i].position.x, walls[i].position.y, walls[i].width, walls[i].height, WHITE);
                    DrawTexture(walls[i].texture, walls[i].position.x - 170, walls[i].position.y - 190, WHITE);
                }
            
            
            EndMode2D();
           
           
           
           //Draws health bar
           DrawRectangle(50, 50, 1000, 40, GRAY);
           DrawRectangle(50, 50, (((float)player.health/(float)player.maxHealth) * 1000), 40, MAROON);
           DrawRectangleLinesEx({50, 50, 1000, 40}, 2,BLACK);
           //draws mana bar
           DrawRectangle(50, 100, player.maxMana*4, 40, GRAY);
           DrawRectangle(50, 100, player.mana * 4, 40, DARKBLUE);
           DrawRectangleLinesEx({50, 100, player.maxMana*4, 40}, 2,BLACK);
           //draws stamina bar
           DrawRectangle(50, 150, player.maxStamina * 5, 40, GRAY);
           DrawRectangle(50, 150, player.stamina * 5, 40, DARKGREEN);
           DrawRectangleLinesEx({50, 150, player.maxStamina*5, 40}, 2,BLACK);
           
           
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
                
                
                int posX = 275;
                int posY = 275;
                
                
                for(int i =0; i < inventory.spells.size(); i++){
                    
                    
                    if(IsMouseButtonPressed(0) && CheckCollisionPointRec({GetMouseX(), GetMouseY()},{inventory.spells[i].position.x, inventory.spells[i].position.y, inventory.spells[i].width, inventory.spells[i].height})){
                        inventoryDragS = true;
                        indexI = i;
                    }
                   
                    
                    if(!inventoryDragS){
                        inventory.spells[i].position.x = posX;
                        inventory.spells[i].position.y = posY;
                    }
                    
                        DrawTexture(inventory.spells[i].spell->texture, inventory.spells[i].position.x, inventory.spells[i].position.y, WHITE);
                    
                    
                    DrawRectangleLines(inventory.spells[i].position.x, inventory.spells[i].position.y, 100, 100, BLACK); 
                    posX += 150;
                    if (posX > 250+(150*8)){
                        posX = 250;
                        posY -= 250;
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
                   for(int i = 0; i < hotBar.spells.size(); i++){
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
                
           }
           
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
               
               DrawRectangleLines(hotBar.spells[i].position.x, hotBar.spells[i].position.y, hotBar.spells[i].width, hotBar.spells[i].height, BLACK);  
               
               if(!(hotBarDrag && indexH == i)){ 
                DrawTexture(hotBar.spells[i].texture, hotBar.spells[i].position.x, hotBar.spells[i].position.y, WHITE);
               }
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
      

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


Vector2 movementRequestS(char axis, int amount, Vector2 position){
    Vector2 result = position;
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
                return result;
           }else{
               //accept the request for movement with no detected collisions
               return {position.x + amount, position.y};
           }
           //returns a Vector2 value for the players new position given the parameters and collisions (y value)
        }else if(axis == 'y' || axis == 'Y'){
            if(CheckCollisionRecs({position.x, position.y+amount, player.width, player.height}, walls[i].rec)){
                
                if(position.y < walls[i].position.y + walls[i].height/2){
                    result.y = walls[i].position.y - player.height;
                }else{
                    result.y = walls[i].position.y + walls[i].height;
                }
                return result;
           }else{
               
               return {position.x, position.y + amount};
           }
        }
       }
       //return statement that does nothing to make the compiler happy
       return {0,0};
}
