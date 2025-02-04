#include "raylib.h"
#include "Spell.h"
class Slot{
    public:
    Texture2D Clear;
    Vector2 position;
    int width; 
    int height;
    Spell *spell = NULL;
    std::string image;
    Texture2D texture;
    Slot(Vector2 position, int width, int height, Spell *spell){
        this->position = position;
        this->width = width;
        this->height = height;
        this->spell = spell;
        
        Clear = LoadTexture("Sprites/Icons/Clear.png");
    }
    Slot(Vector2 position, int width, int height, std::string image){
        this->position = position;
        this->width = width;
        this->height = height;
        this->image = image;
        texture = LoadTexture(image.c_str());
        
        Clear = LoadTexture("Sprites/Icons/Clear.png");
    }
    void addSpell(Spell *spell){
        this->spell = spell;
        texture = spell->texture;
    }
    void removeSpell(){
        spell = NULL;
        texture = Clear;
    }
};