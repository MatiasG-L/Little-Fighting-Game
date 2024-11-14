#include "raylib.h"

class Platform{
    public:  
    // variable that represents the platforms width and height
    float width;
    float height;
    //creating of a Vector2 struct that holds the platforms x and y coordinates
    Vector2 position;
    //Rectangle struct used for the DrawRectanglePro method
    Rectangle rec;
    
    //Parametarized constructor for the platform class
    Platform(float posX, float posY, float width, float height){
        //sets the instance variables to the given paramaters
        this->width = width;
        this->height = height;
        position.x = posX;
        position.y = posY;
        //updates the Rec with the platforms newly set variable values
        rec = {posX, posY, width, height};
    }
    
};