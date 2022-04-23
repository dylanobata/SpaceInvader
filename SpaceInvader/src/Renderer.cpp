#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Shader &shader){
    this->shader = shader;
    this->InitRenderData();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Renderer::InitRenderData() {
    float vertices[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        2.0f, 1.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::Draw(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    // transformations are applied right to left so the last transformation sequentially is actually the first
    // ie 1. scale     2. rotate     3. translate
    
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    
    // lastly translate
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // then rotate: first translate the center of orgin to the centroid, then rotate about the new COR, then translate back
    // Note: the original center of rotation is centered at the left most vertex (0.0f, 1.0f)
    
    // centroid of triangle = ( (x1 + x2 + x3)/3 , (y1 + y2 + y3)/3 )
    float xcentroid = (0.0f + 1.0f + 2.0f) / 3.0f; // x coordinates of vertices from vertices array above
    float ycentroid = (1.0f + 0.0f + 1.0f) / 3.0f; // y coordinates of vertices from vertices array above
    model = glm::translate(model, glm::vec3(xcentroid * size.x, ycentroid * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-xcentroid * size.x, -ycentroid * size.y, 0.0f));
    
    // first scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.setMat4("model", model);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
