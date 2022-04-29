#pragma once

#include <vector>

#include "Game_Object.h"
#include "Renderer.h"

class GameLevel {
    public:
        std::vector<GameObject> Invaders;
        GameLevel(unsigned int levelWidth, unsigned int levelHeight);
        void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
        bool IsCompleted();
        void Draw(Renderer &renderer);
        void Update(float dt, float width);
        void FillBullets(std::vector<GameObject> &enemyBullets);
    private:
        unsigned int levelWidth, levelHeight;
        void Init(std::vector<std::vector<unsigned int>> invaderData);
};
