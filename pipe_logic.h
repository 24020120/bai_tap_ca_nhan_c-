#ifndef GAME_H
#define GAME_H

#include "defs.h"
#include "graphic.h"
#include "pipe_logic.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Graphics* graphics;
    PipeLogic* pipeLogic;

public:
    Game();
    ~Game();

    bool init();
    void handleEvents();
    void update();
    void render();
    void cleanup();
    bool isRunning() const { return running; }
};

#endif
