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

#include "Platform.h"
#include "raylib.h"
#include "Player.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

std::vector<Platform> collidables;

// used to render the level or main menu
bool menu = true;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    //creating variables to set the screen dimentions to
    const int screenWidth = 1600;
    const int screenHeight = 900;
    //varibles for the debugging text
    char xposition[20];
    char yposition[20];
    char groundState[20];
    
    InitWindow(screenWidth, screenHeight, " SQUARED IN++ "); //initilisation of the window 

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    
    Player player;// creation the player object
    
    //creates a few platforms to start off the program with
    Platform platform1(0, 800, 1200, 100);
    Platform platform2(200, 100, 400, 300);

    //vector of platform objects to be checked for collision
    collidables.push_back(platform1);
    collidables.push_back(platform2);
    
    //vector for the player spawn
    Vector2 spawn = {100,100};

    //limits fps for more univarsal experience
    SetTargetFPS(60);
    
    //setting the player speed
    player.setSpeed(10);
      
      
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       
            // player input for moving 
            if (IsKeyDown(KEY_RIGHT) && player.xVelocity < player.getSpeed()) player.xVelocity += player.acceleration;
            else if(IsKeyDown(KEY_LEFT) && player.xVelocity > -player.getSpeed()) player.xVelocity -= player.acceleration;
            else if(player.xVelocity > 0) player.xVelocity /= player.Friction;
            else if(player.xVelocity < 0) player.xVelocity /= player.Friction;
         
         
         //player input for jump
         if(IsKeyDown(KEY_UP) && player.isGrounded) {player.set_yVelocity(player.jumpStr+abs(player.xVelocity)/2);player.isGrounded = false;}
         //player input to reset
         if(IsKeyPressed(KEY_R)){
             player.position = spawn; 
             player.isGrounded = false; 
             player.set_yVelocity(0);
       
         }
         //updating the player Rec to have accurate visuals and collosion
         player.Rec = {player.position.x, player.position.y, player.width, player.height};
         
         
         
      
         
        // collision
        bool grounded = false;
        for(Platform collider : collidables){
            
            // wall collision
             if(player.position.x < collider.position.x + collider.width && (player.position.y > collider.position.y - player.width/2 && player.position.y < collider.position.y + collider.height - 40) && player.position.x > collider.position.x + collider.width/2){
                 player.position.x = collider.position.x + collider.width + 1;
             }
             if(player.position.x > collider.position.x - player.width && (player.position.y > collider.position.y - player.width/2 && player.position.y < collider.position.y + collider.height - 40) && player.position.x < collider.position.x + collider.width/2){
                 player.position.x = collider.position.x - player.width - 1;
             }
             
            //floor collision
             if(player.position.x < collider.position.x + collider.width && player.position.x > collider.position.x - player.width){
                 if((player.position.y > collider.position.y - player.height-1 && player.position.y < collider.position.y + collider.height/2)) {
                     if(CheckCollisionRecs(player.Rec, collider.rec)){
                        player.position.y = collider.position.y - player.height;
                        player.isGrounded = true;
                     }
                     grounded = true;
                 }
             }
            
            //roof collision
            if(player.position.x < collider.position.x + collider.width && player.position.x > collider.position.x - player.width){
                 if((player.position.y > collider.position.y + collider.height -20)) {
                     if(CheckCollisionRecs(player.Rec, collider.rec)){
                        player.position.y = collider.position.y + collider.height;
                        player.set_yVelocity(0);
                     }
                 }
             }
            
        }
        if(!grounded) player.isGrounded = false;
        
        
        if(!player.isGrounded) player.set_yVelocity(player.get_yVelocity() - player.getGravity());
            else player.set_yVelocity(0);
            player.position.y -= player.get_yVelocity();
            player.position.x += player.xVelocity;
            
         //outputs player varibles for debbuging
         sprintf(groundState, "Ground state: %d", player.isGrounded);   
         sprintf(xposition, "yVelocity: %f", player.get_yVelocity());   
         sprintf(yposition, "y:  %f", player.position.y);
            
            
      // Draw, where the scene actually gets rendered and drawn out

        BeginDrawing();

            ClearBackground(LIGHTGRAY);
            
       
            
            //draws a vector of plafrom objects
             for(int i = 0; i < collidables.size(); i++){
                 //updates the objects rec for accurate visuals
                 collidables[i].rec = {collidables[i].position.x, collidables[i].position.y, collidables[i].width, collidables[i].height};
                 //draws each platform at the index of the for loop
                 DrawRectanglePro(collidables[i].rec, {0,0}, 0, DARKGRAY);
                 
                    //circles to show the corners of a platform
                    DrawCircle(collidables[i].position.x , collidables[i].position.y , 10, BLACK);
                    DrawCircle(collidables[i].position.x + collidables[i].width, collidables[i].position.y, 10, BLACK);
                    DrawCircle(collidables[i].position.x + collidables[i].width, collidables[i].position.y + collidables[i].height, 10, BLACK);
                    DrawCircle(collidables[i].position.x, collidables[i].position.y + collidables[i].height, 10, BLACK);
                 
             }
             
             
             //draws player
             DrawRectanglePro(player.Rec, {0,0}, 0, BLACK);
             //updates the players corners with his position
             player.corners = {{player.position.x, player.position.y}, {player.position.x + player.width, player.position.y}, {player.position.x + player.width, player.position.y + player.height}, {player.position.x, player.position.y + player.height}};
             
             //draws the exit door
            
        
                //draws player text that displays the players y coordinate, y velocity, and Grounded state for debbuging
                DrawText(xposition, 100, 100, 30, BLACK); 
                DrawText(yposition, 100, 200, 30, BLACK); 
                DrawText(groundState, 100, 300, 30, BLACK);
                     
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
