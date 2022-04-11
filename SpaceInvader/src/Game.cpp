#include "Game.h"
#include "Shader.h"
#include "Renderer.h"
#include "Game_Object.h"

#include <iostream>

Shader *shader; 
Renderer *renderer;
GameObject *player;

const float velocity = 200.0f;

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
    player = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(30.0f, 25.0f), 0.0f, false);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    renderer->shader.Use();
    renderer->shader.setMat4("projection", projection);
}

void Game::Update(float dt) {
    
}

void Game::ProcessInput(float dt) {
    if (this->State == GAME_ACTIVE) {
        if (this->Keys[GLFW_KEY_LEFT]) {
            // distance = rate * time 
            float distance = velocity * dt;
            if (player->position.x >= 0.0f) {
                player->position.x -= distance;
                std::cout << player->position.x << std::endl;
            }
        }

        if (this->Keys[GLFW_KEY_RIGHT]) {
            float distance = velocity * dt; 
            if (player->position.x - player->size.x <= this->Width) {
                player->position.x += distance;
                std::cout << player->position.x << std::endl;
            }
        }

        if (this->Keys[GLFW_KEY_SPACE]) {

        }
    }
}

void Game::Render() {
    renderer->Draw(player->position, player->size, player->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}
