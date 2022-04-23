#include "Game_Level.h"

#include <iostream>
#include <fstream>
#include <sstream>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int invaderCode;
    GameLevel level;
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
            this->Init(invaderData, levelWidth, levelHeight);
    }
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> invaderData, unsigned int lvlWidth, unsigned int lvlHeight)
{
    unsigned int height = invaderData.size();
    unsigned int width = invaderData[0].size();
    float unit_width =  lvlWidth / static_cast<float>(width);
    float unit_height = lvlWidth / static_cast<float>(height);
    std::cout << lvlWidth << std::endl;
    std::cout << lvlHeight << std::endl;
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (invaderData[y][x] == 1)
            {
                glm::vec2 pos(unit_width*x, unit_height*y);
                glm::vec2 size(15.0f, 10.0f);
                this->Invaders.push_back(GameObject(pos, size, 0.0f, false));
                std::cout << unit_width << " " << unit_height << std::endl;
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

void GameLevel::Draw(Renderer &renderer)
{
    for (GameObject &invader : this->Invaders)
        if (!invader.destroyed)
            renderer.Draw(invader.position, invader.size, invader.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}