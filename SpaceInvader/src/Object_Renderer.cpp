#include "Object_Renderer.h"

void ObjectRenderer::initRenderData()
{
    unsigned int VBO;
    float vertices[] = {
         385.0f,    580.0f,    0.0f,
         400.0f,    565.0f,    0.0f,
         415.0f,    580.0f,    0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
}


