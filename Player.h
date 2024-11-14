#include "raylib.h"


    //---------------------------------------------------------------------------//
   //  NOTE:                                                                    //            
  //      the players origin is at the top left corner of the drawn square     //
 //                                                                           // 
//---------------------------------------------------------------------------//


class Player{
    private:
    float speed; //variable that represents the players max speed
    float yVelocity = 0;// varible that shows the playes Y velocity *how fast the player is moving in the y axis*
    float Gravity = 1; // varible that represents gravity used on the yVelocity variable to bring the player down
    
    public:
    // player attributes *scale together to maintan the apperance of the player square*
    float width = 100;
    float height = 200;
    float Friction = 1.15;//the player stoips over time NOT instantly and this variable is how fast or slow the player stops
    Rectangle Rec; //Rectangle struct used for the DrawRectanglePro method
    float jumpStr = 20;//the strength or height the player can jump *player's jump strangth is increased with x velocity* 
    float acceleration = 0.5;// how fast the player moves from static to full speed *anything 2 or higher is instant*
    
    // player state
    float xVelocity = 0; //varible that represents how fast or how much the player is moving along the x axis *this value cant surpass the speed varible*
    bool isGrounded; //varible to represent if the player is grounded used for collision and gravity logic
    Vector2 position;// struct contaning the players x and y coordinate values
    //decleration of a struct that hold the vectors *coordinates* of the players corners used for collision
    typedef struct Verticies{
        Vector2 TopLeft;//vector of the top left corner of the player
        Vector2 TopRight;//vector of the top right corner of the player
        Vector2 BottomRight;//vector of the bottom right corner of the player
        Vector2 BottomLeft;//vector of the bottom left corner of the player
    } Verticies;
    Verticies corners; //creating an instance of the previously declared struct  
    
    //non paramaterized constructer of the player class
    Player(){
        //initilization of some instance variables
        position.x = 100;
        position.y = 100;
        speed = 2;
        isGrounded = false;
        //updates the Rec with the players newly set variable values
        Rec = {position.x, position.y, width, height};
        //updates the corners instance with the players newly ste varible values
        corners = {{position.x, position.y}, {position.x + width, position.y}, {position.x + width, position.y + height}, {position.x, position.y + height}};
    }
    
 // accessor and mutator methods for the players private variables
    float getSpeed(){
        return speed;
    }
    void setSpeed(float speed){
        this->speed = speed;
    }
    float get_yVelocity(){
        return yVelocity;
    }
    void set_yVelocity(float yVelocity){
        this->yVelocity = yVelocity;
    }
    float getGravity(){
        return Gravity;
    }
    void setGravity(float Gravity){
        this->Gravity = Gravity;
    }
    
};