#pragma once

#include <glm/glm.hpp>

class GameObject 
{
public:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 velocity;

    GameObject();
    ~GameObject();
};
