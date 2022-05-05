#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "Renderer.h"

class GameObject {
    public:
        glm::vec2 position, size;
        glm::vec3 color;
        float rotation; // orientation amount of object
        bool destroyed; // check whether object has been hit or not
        bool direction; // used for enemy objects to determine whether to move left or right
        Texture2D Sprite;

        GameObject() { }
        GameObject(glm::vec2 pos, glm::vec2 sz, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), float r=0.0f, bool destroy = false, bool dir = true); 
};
