#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"

class GameObject {
    public:
        glm::vec2 position, size;
        float rotation;
        bool destroyed;
        
        GameObject() { };
        GameObject(glm::vec2 pos, glm::vec2 sz, float r, bool destroy); 
};
