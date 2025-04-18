#include "raylib.h"

class Wall{
    public:  
    // variable that represents the platforms width and height
    float width;
    float height;
    //creating of a Vector2 struct that holds the platforms x and y coordinates
    Vector2 position;
    //Rectangle struct used for the DrawRectanglePro method
    Rectangle rec;
    
    //texture to make object have a texture
    Texture2D texture;
    
    //Parametarized constructor for the platform class
    Wall(float posX, float posY, float width, float height, Texture2D texture){
        //sets the instance variables to the given paramaters
        this->width = width;
        this->height = height;
        position.x = posX;
        position.y = posY;
        //updates the Rec with the platforms newly set variable values
        rec = {posX, posY, width, height};
        
        this->texture = texture;
    }
    
};