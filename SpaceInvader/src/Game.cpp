#include "Game.h"
#include "Shader.h"
#include "Renderer.h"
#include "Game_Object.h"

#include <iostream>
#include <vector>

Shader *shader; 
Renderer *renderer;
GameObject *player;
std::vector<GameObject> bullets;

const float velocity = 300.0f;
const float bullet_velocity = 1000.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height) { }

Game::~Game()
{
    delete shader;
    delete renderer;
    delete player;
}

void Game::Init() {
    shader = new Shader("W:/SpaceInvader/SpaceInvader/res/Shaders/Sprite.vert", "W:/SpaceInvader/SpaceInvader/res/Shaders/Sprite.frag");
    renderer = new Renderer(*shader);
    player = new GameObject(glm::vec2(static_cast<float>(this->Width - 30.0f)/2.0f, 550.0f), glm::vec2(30.0f, 25.0f), 0.0f, false);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    renderer->shader.Use();
    renderer->shader.setMat4("projection", projection);
}

void Game::Update(float dt) {
    // update bullet positions
    for (auto bullet : bullets) {
        float distance = bullet_velocity * dt;
        bullet.position.y += distance;
        std::cout << "Update: " << bullet.position.y << " " << bullets.size() << std::endl;
    }
}

void Game::ProcessInput(float dt) {
    if (this->State == GAME_ACTIVE) {
        if (this->Keys[GLFW_KEY_LEFT] || this->Keys[GLFW_KEY_A]) {
            // distance = rate * time
            float distance = velocity * dt;
            if (player->position.x >= 0.0f) {
                player->position.x -= distance;
            }
        }

        if (this->Keys[GLFW_KEY_RIGHT] || this->Keys[GLFW_KEY_D]) {
            float distance = velocity * dt;
            // position.x is defined to be the leftmost vertex of the player so we need to add size to get rightmost vertex of player
            if (player->position.x + player->size.x < this->Width - player->size.x) {
                player->position.x += distance;
            }
        }
        
        if (this->Keys[GLFW_KEY_SPACE]) {
            // create new bullets
            glm::vec2 bulletPosition = glm::vec2((player->position.x + player->size.x - 0.75f), player->position.y - 5.0f);
            GameObject* bullet = new GameObject(bulletPosition, glm::vec2(0.75f, 5.0f), 0.0f, false);
            bullets.push_back(*bullet);
        }
    }

}

void Game::Render() {
    renderer->Draw(player->position, player->size, player->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    for (auto bullet : bullets) {
        renderer->Draw(bullet.position, bullet.size, bullet.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        std::cout << "Render: " << bullet.position.y << " " << bullets.size() << std::endl;
    }
}
