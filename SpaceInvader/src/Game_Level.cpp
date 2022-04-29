#include "Game_Level.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

GameLevel::GameLevel(unsigned int width, unsigned int height)
    : levelWidth(width), levelHeight(height) { }

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int invaderCode;
    GameLevel level(levelWidth, levelHeight);
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> invaderData;
    if (fstream)
    {
        while (std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> invaderCode)
                row.push_back(invaderCode);
            invaderData.push_back(row);
        }
        if (invaderData.size() > 0)
            this->Init(invaderData);
    }
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> invaderData)
{
    unsigned int height = invaderData.size();
    unsigned int width = invaderData[0].size();
    float unit_width =  this->levelWidth / static_cast<float>(width);
    float unit_height = this->levelWidth / static_cast<float>(height);
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (invaderData[y][x] == 1)
            {
                float xoffset = 30;
                float yoffset = 10;
                glm::vec2 pos(80*x + 60, 60*y);
                glm::vec2 size(15.0f, 10.0f);
                this->Invaders.push_back(GameObject(pos, size, 180.0f, false));
            }
        }
    }
}

bool GameLevel::IsCompleted()
{
    for (GameObject &invader : Invaders)
        if (!invader.destroyed)
            return false;
    return true;
}

void GameLevel::Update(float dt, float width)
{
    if (Invaders[0].position.x <= 0)
        Invaders[0].direction = false;
    
    if (Invaders.back().position.x + Invaders[0].size.x >= width - Invaders[0].size.x)
        Invaders[0].direction = true;

    float velocity = 50.0f;
    float distance = velocity * dt;
    for (unsigned int i = 0; i < Invaders.size(); i++) {
        if (Invaders[0].direction) { 
            Invaders[i].position.x -= distance;
        }
        else {
            Invaders[i].position.x += distance;
        }
    }
}

void GameLevel::FillBullets(std::vector<GameObject> &enemyBullets)
{
    if (enemyBullets.size() < 20) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, Invaders.size()-1);
        for (int i = 0; i < 20; i++) {
            int n = distr(gen); 
            if (!Invaders[n].destroyed) {
                glm::vec2 bulletPosition = glm::vec2((Invaders[n].position.x + Invaders[n].size.x / 2.0f), Invaders[n].position.y - 5.0f);
                GameObject bullet = GameObject(bulletPosition, glm::vec2(0.75f, 5.0f), 0.0f, false);
                enemyBullets.push_back(bullet);
            }
        }
    }
    else {
        if (enemyBullets.back().position.y > levelHeight+500)
            enemyBullets.clear();
    }
}

void GameLevel::Draw(Renderer &renderer)
{
    for (GameObject &invader : this->Invaders)
        if (!invader.destroyed)
            renderer.Draw(invader.position, invader.size, invader.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}
