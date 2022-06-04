#include "Game.h"
#include "Resource_Manager.h"
#include "Renderer.h"
#include "Game_Object.h"
#include "Text_Renderer.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>

Renderer *renderer;
TextRenderer *Text;
GameObject *player;
std::vector<GameObject> bullets, enemyBullets;

const float PLAYER_VELOCITY = 200.0f;
const float BULLET_VELOCITY = 300.0f;
const unsigned int MAX_BULLETS = 20;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_START), Width(width), Height(height) { }

Game::~Game()
{
    delete renderer;
    delete player;
    delete Text;
}

/*
In: None
Out: None
Function: Responsible for loading all assessts of the game. Uses ResourceManager class to load textures and shaders for use.
          Initializes Renderer object, player GameObject, GameLevel objects, and TextRenderer object.
*/
void Game::Init() 
{
    ResourceManager::LoadShader("res/Shaders/Sprite.vert", "res/Shaders/Sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width),
                                      static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader spriteShader = ResourceManager::GetShader("sprite");
    renderer = new Renderer(spriteShader);

    // Player object
    ResourceManager::LoadTexture("res/Textures/laserBullet.png", true, "bullet");
    ResourceManager::LoadTexture("res/Textures/shuttle2.png", true, "player");
	player = new GameObject(glm::vec2(static_cast<float>(Width - 30.0f)/2.0f, 550.0f), glm::vec2(15.0f, 10.0f),
                            ResourceManager::GetTexture("player")); 
	// Game Level	
	GameLevel one(Width, Height);
    one.Load("res/Levels/one.lvl", Width, Height);
    Levels.push_back(one);
    Level = 0;
    
    // Text
    Text = new TextRenderer(Width, Height);
    Text->Load("res/Fonts/ocraext.TTF", 24);
    
}

/*
In: Two GameObject types
Out: boolean
Function: Checks to see if/when two gameobjects collide. Collisions occur when
          the right endpoint of an object is greater than or equal to the left
          endpoint of the other object but the left endpoint of the first object
          is smaller than the right endpoint of the second object.
          Similarly for the vertical axis. Collisions occur when both are true.
*/
bool Game::CheckCollision(GameObject &one, GameObject &two) 
{
    bool collisionX = one.position.x + one.size.x >= two.position.x 
                    && two.position.x + two.size.x >= one.position.x;
    bool collisionY = one.position.y + one.size.y >= two.position.y 
                    && two.position.y + two.size.y >= one.position.y;
    return collisionX && collisionY;
}

/*
In: None
Out: None
Function: Check if player hits invader or is hit by invader (or invader bullet) using the CheckCollision
          function above. Iterates through Invaders vector of the given level as well as the bullets they shoot.
          If enemy is destroyed, set the destroyed attribute to true. If player is destroyed, set the game state
          to GAME_OVER
*/
void Game::DoCollisions()
{
    // check if invaders are hit by player bullet
    for (GameObject &invader : Levels[Level].Invaders)
    {
        if (!invader.destroyed) {
            for (GameObject &bullet : bullets) 
            {
                if (CheckCollision(bullet, invader))
                {
                    invader.destroyed = true;
                    bullet.destroyed = true;
                }
            }
        }
    }
    // check if player is hit by enemy bullet
    for (GameObject &bullet : enemyBullets)
    {
        if (CheckCollision(*player, bullet)) 
        {
            player->destroyed = true;
            bullet.destroyed = true;
        }
    }
    
    // check if player is hit by invader
    for (GameObject &invader : Levels[Level].Invaders)
    {
        if (!invader.destroyed) 
        {
            if (CheckCollision(*player, invader))
            {
                invader.destroyed = true;
                player->destroyed = true;
                State = GAME_OVER;
            }
        }
    }
}

/*
In: float (delta time)
Out: None
Function: 
*/
void Game::ProcessInput(float dt)
{
    if (State == GAME_START)
    {
        if (Keys[GLFW_KEY_ENTER])
        {
            State = GAME_ACTIVE;
            Keys[GLFW_KEY_ENTER] = false;
        }
    }
    if (State == GAME_ACTIVE)
    {
        // left arrow or a key used to register left player movement
        if (Keys[GLFW_KEY_LEFT] || Keys[GLFW_KEY_A]) 
        {
            // distance = rate * time
            float distance = PLAYER_VELOCITY * dt;
            if (player->position.x >= 0.0f) 
            {
                player->position.x -= distance;
            }
        }
        // right arrow or d key used to register right player movement
        if (Keys[GLFW_KEY_RIGHT] || Keys[GLFW_KEY_D]) 
        {
            float distance = PLAYER_VELOCITY * dt;
            // position.x is defined to be the leftmost vertex of the player so we need to add size to get rightmost vertex of player
            if (player->position.x + player->size.x < Width ) 
            {
                player->position.x += distance;
            }
        }
        // space key is used to register bullets
        if (Keys[GLFW_KEY_SPACE]) 
        {
            Keys[GLFW_KEY_SPACE] = false; // reset to false evertime space is hit. Otherwise too many bullet objects are made
            if (bullets.size() < MAX_BULLETS) 
            {
                glm::vec2 bulletPosition = glm::vec2((player->position.x + player->size.x / 2.0f), player->position.y - 5.0f);
                GameObject bullet = GameObject(bulletPosition, glm::vec2(20.0f, 20.0f), ResourceManager::GetTexture("bullet"));
                bullets.push_back(bullet);
            }
            else 
            { // check to see if last bullet shot is off screen before clearing
                if (bullets.back().position.y <= 0)
                    bullets.clear();
            }
        }
        if (Keys[GLFW_KEY_ENTER]) // pauses game and enters Game menu
        {
            State = GAME_MENU;
            Keys[GLFW_KEY_ENTER] = false;
        }
    }

    if (State == GAME_MENU)
    {
        if (Keys[GLFW_KEY_ENTER])
        {
            State = GAME_ACTIVE;
            Keys[GLFW_KEY_ENTER] = false;
        }
    }

    if (State == GAME_OVER)
        if (Keys[GLFW_KEY_ENTER])
        {
            State = GAME_ACTIVE; // if ENTER is hit, restart the game

        }
}

/*
In: float (delta time)
Out: None
Function: Uses dt and VELOCITY to calculate the change of distance over a small interval
          for each bullet. Updates the global bullets vector accordingly.
*/
void Game::UpdateBulletPosition(float dt)
{
    float distance = BULLET_VELOCITY * dt;
    for (GameObject &bullet : bullets)
    {
        bullet.position.y -= distance;
    }
    Levels[Level].FillBullets(enemyBullets);
    for (GameObject &bullet : enemyBullets) 
    {
        bullet.position.y += distance;
    }
}

/*

*/ 
void Game::ResetLevel()
{
    Levels[0].Clear();
    bullets.clear();
    enemyBullets.clear();
    if (Level == 0)
        Levels[0].Load("res/Levels/one.lvl", Width, Height);
}

bool AreDestroyed(GameObject invader)
{
    return invader.destroyed;
}

void Game::Update(float dt)
{
    if (State == GAME_ACTIVE) 
    {
        UpdateBulletPosition(dt);
        Levels[Level].Update(dt, Width);
        DoCollisions();
        if (player->destroyed)
        {
            State = GAME_OVER;
        }
        else if (all_of(Levels[Level].Invaders.begin(), Levels[Level].Invaders.end(), AreDestroyed))
        {
            State = GAME_WIN;
        }
    }

    if (State == GAME_OVER || State == GAME_WIN)
        if (Keys[GLFW_KEY_ENTER])
        {
            ResetLevel();
            Init();
            State = GAME_ACTIVE;
        }
}

void Game::Render() {
    if (State == GAME_START)
    {
        
        Text->RenderText("Press ENTER to start or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
        Text->RenderText("Use the left & right arrow keys or a & d keys to move left and right. Hit SPACE to shoot.", 50.0, Height / 2 + 25.0f, 0.5f);
        if (!player->destroyed) 
        {
            renderer->Draw(player->Sprite, player->position, player->size, player->rotation, player->color);
        }

        for (GameObject &bullet : bullets)
            if (!bullet.destroyed && bullet.position.y < Height)
            renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
        
        for (GameObject &bullet : enemyBullets)
            if (!bullet.destroyed && bullet.position.y < Height)
            renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
        
        Levels[Level].Draw(*renderer);
    }

    if (State == GAME_ACTIVE) 
    {
        if (!player->destroyed) 
        {
            renderer->Draw(player->Sprite, player->position, player->size, player->rotation, player->color);
        }

        for (GameObject &bullet : bullets)
            if (!bullet.destroyed && bullet.position.y < Height)
            renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
        
        for (GameObject &bullet : enemyBullets)
            if (!bullet.destroyed && bullet.position.y < Height)
            renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
        
        Levels[Level].Draw(*renderer);
    }

    if (State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
        if (!player->destroyed) 
        {
            renderer->Draw(player->Sprite, player->position, player->size, player->rotation, player->color);
        }

        for (GameObject &bullet : bullets)
            if (!bullet.destroyed && bullet.position.y < Height)
            renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
        
        for (GameObject &bullet : enemyBullets)
            if (!bullet.destroyed && bullet.position.y < Height)
            renderer->Draw(bullet.Sprite, bullet.position, bullet.size, bullet.rotation, bullet.color);
        
        Levels[Level].Draw(*renderer);
    }
    
    if (State == GAME_WIN)
    {
        if (!player->destroyed) 
        {
            renderer->Draw(player->Sprite, player->position, player->size, player->rotation, player->color);
        }
        Text->RenderText("YOU WIN!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
        Text->RenderText("Press ENTER to play again or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
    }
    
    if (State == GAME_OVER)
    {
        Text->RenderText("GAME OVER", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
    }
}
