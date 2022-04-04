#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class ObjectRenderer
{
public:
    ObjectRenderer();
    ~ObjectRenderer();
    void DrawObject(glm::vec2 position, glm::vec2 size, 
                    float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    unsigned int quadVAO;
    void initRenderData();
};
