#include "raylib.h"
class projectile{
    public:
    Vector2 position;
    int range;
    Vector2 velocity;
    int power;
    char SPfactor;
    int width;
    int height;
    Texture2D image;
    Vector2 start;
    float rotation;
    float speed;
    float knockback;
    bool freindly = true;
    projectile(Vector2 position, int range, Vector2 velocity, int power, char SPfactor, int width, int height, Texture2D image, float rotation, float speed, float knockback){
        this->position = position;
        start = position;
        this->range = range;
        this->velocity = velocity;
        this->power = power;
        this->SPfactor = SPfactor;
        this->width = width;
        this->height = height;
        this->image = image;
        this->rotation = rotation;
        this->speed = speed;
        this->knockback = knockback;
    }
    
    projectile(Vector2 position, int range, Vector2 velocity, int power, char SPfactor, int width, int height, Texture2D image, float rotation, float speed, float knockback, bool freindly){
        this->position = position;
        start = position;
        this->range = range;
        this->velocity = velocity;
        this->power = power;
        this->SPfactor = SPfactor;
        this->width = width;
        this->height = height;
        this->image = image;
        this->rotation = rotation;
        this->speed = speed;
        this->knockback = knockback;
        this->freindly = freindly;
    }
};