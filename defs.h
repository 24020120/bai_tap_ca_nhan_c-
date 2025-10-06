#ifndef DEFS_H
#define DEFS_H
#include <SDL.h>
#include <vector>
#include "pipe.h"


const int TS = 54;
const SDL_Point OFFSET = {65, 55};
const SDL_Point DIR[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

const int START = 0;
const int GUIDE = 1;
const int SETTINGS = 2;
const int EXIT = 3;
const int NONE = 4;
const int REPLAY = 5;

extern int gridSize;
extern int winSize;
extern bool mute;
extern std::vector<std::vector<Pipe>> grid;

void genGrid();
Pipe& getPipe(int x, int y);
bool out(const SDL_Point& pos);
void flood(const SDL_Point& pos);

int showMenu(SDL_Renderer* ren);
int showSettings(SDL_Renderer* ren, bool& mute);
int showGuide(SDL_Renderer* ren);
int showWin(SDL_Renderer* ren);

#endif
