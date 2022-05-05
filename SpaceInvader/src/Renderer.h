#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class Renderer {
public:
    Renderer(Shader &shader);
    ~Renderer();
    
    void Draw(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate = 0.0f,
              glm::vec3 color = glm::vec3(0.5f));
private:
    Shader shader;
    unsigned int VAO;
    void InitRenderData();
};
