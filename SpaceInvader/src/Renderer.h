#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class Renderer {
public:
    Renderer(Shader &shader);
    ~Renderer();

    void Draw(glm::vec2 position, glm::vec2 size, float rotate = 0.0f,
              glm::vec3 color = glm::vec3(0.5f));
    Shader shader;
    unsigned int VAO;
    void InitRenderData();
};
