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

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

bool movementRequest(char axis, int amount, Vector2 position);
Vector2 movementRequestS(char axis, int amountY, Vector2 position);

std::vector<Wall> walls;

// used to render the level or main menu
bool menu = true;

int main(void)
{
    
    // Initialization
    //--------------------------------------------------------------------------------------
    //creating variables to set the screen dimentions to
    const int screenWidth = 1600;
    const int screenHeight = 900;
    
    Wall wall1(200, 200, 300, 500);
    walls.push_back(wall1);
    
    InitWindow(screenWidth, screenHeight, " SQUARED IN++ "); //initilisation of the window 

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    
    Player player;// creation the player object

    //limits fps for more univarsal experience
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    float timerWalk = 0;
    float timerRegen = 0;
    std::cout << "\n" << player.health << "\n";
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        //player movement when holding down the keys
        
        if(timerRegen < 0.5){
            timerRegen += GetFrameTime();
        }else if(player.stamina < player.maxStamina){
            player.stamina += 5;
            timerRegen = 0;
        }
        
        if(timerWalk < 0.1){
            timerWalk += GetFrameTime();
        }else{
            if(IsKeyDown(KEY_D)){
                player.position = movementRequestS('x', player.stats.agility * 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('x', player.stats.agility * 15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            if(IsKeyDown(KEY_A)){
                player.position = movementRequestS('x', -player.stats.agility * 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('x', -player.stats.agility * 15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            if(IsKeyDown(KEY_W)){
                player.position = movementRequestS('Y', -player.stats.agility * 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('y', -player.stats.agility * 15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            if(IsKeyDown(KEY_S)){
                player.position = movementRequestS('y', player.stats.agility * 15, player.position);
                if(IsKeyDown(KEY_LEFT_SHIFT) && player.stamina >= 0){
                    player.position = movementRequestS('y', player.stats.agility * 15, player.position);
                    player.stamina -= 2;
                    timerRegen = 0;
                }
            }
            timerWalk = 0;
        }
       //playermovement when pressing the keys
       if(IsKeyPressed(KEY_D)){
           player.position = movementRequestS('x', player.stats.agility * 15, player.position);
       }
       if(IsKeyPressed(KEY_A)){
           player.position = movementRequestS('x', -player.stats.agility * 15, player.position);
       }
       if(IsKeyPressed(KEY_W)){
           player.position = movementRequestS('Y', -player.stats.agility * 15, player.position);
       }
       if(IsKeyPressed(KEY_S)){
           player.position = movementRequestS('y', player.stats.agility * 15, player.position);
       }
            
            
        if(IsKeyPressed(KEY_R)){
            player.health -= 5;
        }
            
      // Draw, where the scene actually gets rendered and drawn out

        BeginDrawing();
            
            
            
            ClearBackground(LIGHTGRAY);
            //draws the player
            DrawRectangleV(player.position, {player.width, player.height}, BLACK);
            //draws the walls
            for(int i = 0; i < walls.size(); i++){
                DrawRectangleV(walls[i].position, {walls[i].width, walls[i].height}, WHITE);
            }
           DrawRectangle(50, 50, player.health * 5, 40, MAROON);
           DrawRectangle(50, 100, player.mana * 4, 40, DARKBLUE);
           DrawRectangle(50, 150, player.stamina * 5, 40, DARKGREEN);
           
        //ends the drawing phase of the program     
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
     
     

    // De-Initialization
    //--------------------------------------------------------------------------------------
      // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


Vector2 movementRequestS(char axis, int amount, Vector2 position){
    Vector2 result = position;
    for(int i = 0; i < walls.size(); i++){
           //returns true or false if the players current position(x) + the amount hes desired to move collides with a wall
        if(axis == 'X' || axis == 'x'){
           if(CheckCollisionRecs({position.x+amount, position.y, 50, 100}, walls[i].rec)){
                if(position.x < walls[i].position.x+walls[i].width/2){
                     result.x = walls[i].position.x - 50;
                }else{
                     result.x = walls[i].position.x + walls[i].width;
                }
                return result;
           }else{
               //accept the request for movement with no detected collisions
               return {position.x + amount, position.y};
           }
        }else if(axis == 'y' || axis == 'Y'){
            if(CheckCollisionRecs({position.x, position.y+amount, 50, 100}, walls[i].rec)){
                
                if(position.y < walls[i].position.y + walls[i].height/2){
                    result.y = walls[i].position.y - 100;
                }else{
                    result.y = walls[i].position.y + walls[i].height;
                }
                return result;
           }else{
               
               return {position.x, position.y + amount};
           }
        }
       }
}
