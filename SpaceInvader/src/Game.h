#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game_Level.h"

enum GAMESTATE {
    GAME_START,
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_OVER
};

class Game {
public: 
    bool Keys[1040];
    
    Game();
    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
 
private:
    GAMESTATE State;
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int Level;
    void ResetLevel();
    bool CheckCollision(GameObject &one, GameObject &two);
    void UpdateBulletPosition(float dt);
};
