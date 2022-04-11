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
    /* 
    float vertices[] = {
        385.0f,  580.0f,
        400.0f,  565.0f,
        415.0f,  580.0f
    };
    */
    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        2.0f, 0.0f
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
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f)); // first translate
    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
	this->shader.setMat4("model", model);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
