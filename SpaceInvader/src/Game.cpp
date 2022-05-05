#include "Game.h"
#include "Resource_Manager.h"
#include "Renderer.h"
#include "Game_Object.h"

#include <iostream>
#include <vector>

Renderer *renderer;
GameObject *player;
std::vector<GameObject> bullets, enemyBullets;

const float PLAYER_VELOCITY = 200.0f;
const float BULLET_VELOCITY = 300.0f;
const unsigned int MAX_BULLETS = 30;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height) { }

Game::~Game()
{
    delete renderer;
    delete player;
}

void Game::Init() {
    ResourceManager::LoadShader("res/Shaders/Sprite.vert", "res/Shaders/Sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader spriteShader = ResourceManager::GetShader("sprite");
    renderer = new Renderer(spriteShader);

    // Player object
    ResourceManager::LoadTexture("res/Textures/laserBullet.png", true, "bullet");
    ResourceManager::LoadTexture("res/Textures/shuttle2.png", true, "player");
	player = new GameObject(glm::vec2(static_cast<float>(this->Width - 30.0f)/2.0f, 550.0f), glm::vec2(15.0f, 10.0f),
                            ResourceManager::GetTexture("player"));
	// Game Level	
	GameLevel one(this->Width, this->Height);
    one.Load("res/Levels/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    
}

bool Game::CheckCollision(GameObject &one, GameObject &two) {
    bool collisionX = one.position.x + one.size.x >= two.position.x 
                    && two.position.x + two.size.x >= one.position.x;
    bool collisionY = one.position.y + one.size.y >= two.position.y 
                    && two.position.y + two.size.y >= one.position.y;
    return collisionX && collisionY;
}

void Game::DoCollisions()
{
    // check if invaders are hit
    for (GameObject &invader : this->Levels[this->Level].Invaders)
    {
        if (!invader.destroyed) {
            for (GameObject &bullet : bullets) {
                if (CheckCollision(bullet, invader))
                {
                    invader.destroyed = true;
                    bullet.destroyed = true;
                }
            }
        }
    }
    // check if player is hit
    for (GameObject &bullet : enemyBullets){
        if (CheckCollision(*player, bullet)) {
            player->destroyed = true;
            bullet.destroyed = true;
        }
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
                glm::vec2 bulletPosition = glm::vec2((player->position.x + player->size.x / 2.0f), player->position.y - 5.0f);
                GameObject bullet = GameObject(bulletPosition, glm::vec2(2.0f, 5.0f), ResourceManager::GetTexture("bullet"));
                bullets.push_back(bullet);
            }
            else { // check to see if last bullet shot is off screen before clearing
                if (bullets.back().position.y <= 0)
                    bullets.clear();
            }
        }
    }
}

void Game::UpdateBulletPosition(float dt)
{
    float distance = BULLET_VELOCITY * dt;
    for (GameObject &bullet : bullets) {
        bullet.position.y -= distance;
    }
    this->Levels[this->Level].FillBullets(enemyBullets);
    for (GameObject &bullet : enemyBullets) {
        bullet.position.y += distance;
    }
}

void Game::Update(float dt) {
    this->UpdateBulletPosition(dt);
    this->Levels[this->Level].Update(dt, this->Width);
    this->DoCollisions();
}

void Game::Render() {
    if (!player->destroyed) {
        renderer->Draw(player->Sprite, player->position, player->size, player->rotation, player->color);
    }

    for (GameObject &bullet : bullets)
        if (!bullet.destroyed && bullet.position.y < this->Height)
        renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
    
    for (GameObject &bullet : enemyBullets)
        if (!bullet.destroyed && bullet.position.y < this->Height)
        renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
    
    this->Levels[this->Level].Draw(*renderer);
}
