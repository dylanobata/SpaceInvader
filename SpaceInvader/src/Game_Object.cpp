#include "Game_Object.h"

GameObject::GameObject(glm::vec2 pos, glm::vec2 sz, Texture2D sprite, 
        glm::vec3 color, float r, bool destroy, bool dir)
    : position(pos), size(sz), Sprite(sprite), color(color), rotation(r), destroyed(destroy), direction(dir) { }


