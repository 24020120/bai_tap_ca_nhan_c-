#ifndef SAVEGAME_H
#define SAVEGAME_H
#include <string>
#include <vector>
#include <SDL.h>
#include "pipe.h"

struct GameState {
    int score;
    int timeRemaining;
    bool mute;
    SDL_Point serverPos;
    std::vector<std::vector<Pipe>> grid;
    int rounds;
    int gridSize;
};

bool saveGame(const GameState& state, const std::string& filename);
bool loadGame(GameState& state, const std::string& filename);
#endif
