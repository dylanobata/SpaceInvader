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

const float PLAYER_VELOCITY = 200.0f;
const float BULLET_VELOCITY = 300.0f;
const unsigned int MAX_BULLETS = 30;

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
    player = new GameObject(glm::vec2(static_cast<float>(this->Width - 30.0f)/2.0f, 550.0f), glm::vec2(15.0f, 10.0f), 0.0f, false);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    renderer->shader.Use();
    renderer->shader.setMat4("projection", projection);
    GameLevel one;
    one.Load("W:/SpaceInvader/SpaceInvader/res/Levels/one.lvl", this->Width / 2, this->Height / 2);
    this->Levels.push_back(one);
    this->Level = 0;
}

void Game::Update(float dt) {
    float distance = BULLET_VELOCITY * dt;
    for (GameObject &bullet : bullets) {
        bullet.position.y -= distance;
    }
}

void Game::ProcessInput(float dt) {
    if (this->State == GAME_ACTIVE) {
        // left arrow or a key used to register left player movement
        if (this->Keys[GLFW_KEY_LEFT] || this->Keys[GLFW_KEY_A]) {
            // distance = rate * time
            float distance = PLAYER_VELOCITY * dt;
            if (player->position.x >= 0.0f) {
                player->position.x -= distance;
            }
        }
        // right arrow or d key used to register right player movement
        if (this->Keys[GLFW_KEY_RIGHT] || this->Keys[GLFW_KEY_D]) {
            float distance = PLAYER_VELOCITY * dt;
            // position.x is defined to be the leftmost vertex of the player so we need to add size to get rightmost vertex of player
            if (player->position.x + player->size.x < this->Width - player->size.x) {
                player->position.x += distance;
            }
        }
        // space key is used to register bullets
        if (this->Keys[GLFW_KEY_SPACE]) {
            this->Keys[GLFW_KEY_SPACE] = false; // reset to false evertime space is hit. Otherwise too many bullet objects are made
            if (bullets.size() < MAX_BULLETS) {
                glm::vec2 bulletPosition = glm::vec2((player->position.x + player->size.x - 0.75f), player->position.y - 5.0f);
                GameObject bullet = GameObject(bulletPosition, glm::vec2(0.75f, 5.0f), 0.0f, false);
                bullets.push_back(bullet);
            }
            else { // check to see if last bullet shot is off screen before clearing
                if (bullets.back().position.y <= 0)
                    bullets.clear();
            }
        }
    }
}

void Game::Render() {
    renderer->Draw(player->position, player->size, player->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    for (GameObject &bullet : bullets)
        renderer->Draw(bullet.position, bullet.size, bullet.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    this->Levels[this->Level].Draw(*renderer);
}
