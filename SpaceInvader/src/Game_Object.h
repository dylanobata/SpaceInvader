#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"

class GameObject {
    public:
        glm::vec2 position, size;
        float rotation; // orientation amount of object
        bool destroyed; // check whether object has been hit or not
        bool direction; // used for enemy objects to determine whether to move left or right

        GameObject() { }
        GameObject(glm::vec2 pos, glm::vec2 sz, float r, bool destroy = false, bool dir = true); 
};
