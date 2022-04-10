#pragma once

enum GAMESTATE {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
public: 
    GAMESTATE State;
    bool Keys[400];
    unsigned int Width, Height;
    Game();
    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};
