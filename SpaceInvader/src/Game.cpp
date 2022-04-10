#include "Game.h"
#include "Shader.h"
#include "Renderer.h"

#include <iostream>

Shader *shader; 
Renderer *renderer;
// GameObject *player

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height) { }

Game::~Game()
{
    delete shader;
    delete renderer;
    // delete player;
}

void Game::Init() {
    shader = new Shader("W:/SpaceInvader/SpaceInvader/res/Shaders/Sprite.vert", "W:/SpaceInvader/SpaceInvader/res/Shaders/Sprite.frag");
    renderer = new Renderer(*shader);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    renderer->shader.Use();
    renderer->shader.setMat4("projection", projection);
}

void Game::Update(float dt) {
    
}

void Game::ProcessInput(float dt) {
    
}

void Game::Render() {
    renderer->Draw(glm::vec2(1.0f, 1.0f), glm::vec2(10.0f, 10.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
