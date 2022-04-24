#include "Game_Object.h"

GameObject::GameObject(glm::vec2 pos, glm::vec2 sz, float r, 
                       bool destroy, bool dir)
    : position(pos), size(sz), rotation(r), destroyed(destroy), direction(dir) { }
