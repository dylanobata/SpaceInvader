#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game_Level.h"

enum GAMESTATE {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
public: 
    GAMESTATE State;
    bool Keys[1040];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int Level;

    Game();
    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
 
private:
    bool CheckCollision(GameObject &one, GameObject &two);
    void UpdateBulletPosition(float dt);
};
