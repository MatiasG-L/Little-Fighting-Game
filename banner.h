#include "raylib.h"
class banner{
    
    public:
    
    float width;
    float height;
    Vector2 position;
    float decayTime;
    float decayTimer = 0;
    std::string text;
    
    banner(float width, float height, Vector2 position, float decayTime, std::string text){
        this->width = width;
        this->height = height;
        this->position = position;
        this->decayTime = decayTime; 
        this->text = text;
    }
    
};