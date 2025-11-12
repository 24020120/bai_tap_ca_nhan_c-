#ifndef DEFS_H
#define DEFS_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "pipe.h"
#include "highscore.h"
class AuthManager;

const int TS = 54;
const SDL_Point OFFSET = {75, 120};
const SDL_Point DIR[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

const int START = 0;
const int GUIDE = 1;
const int SETTINGS = 2;
const int EXIT = 3;
const int NONE = 4;
const int REPLAY = 5;
const int LOGIN = 6;
const int REGISTER = 7;
const int CONTINUE_GAME = 8;
const int EXIT_FROM_PAUSE = 9;
const int ITEM_REMOVE_COMPUTER = 10;
const int ITEM_FIX_GLASS = 11;
const int ITEM_ADD_TIME = 12;
extern int itemRemoveComputer;
extern int itemFixGlass;
extern int itemAddTime;
extern int score;
extern int highScore;
extern int gridSize;
extern int winSize;
extern bool mute;
extern int itemRemoveComputer;
extern int itemFixGlass;
extern int itemAddTime;
extern std::vector<std::vector<Pipe>> grid;

void genGrid();
Pipe& getPipe(int x, int y);
bool out(const SDL_Point& pos);
void flood(const SDL_Point& pos);
bool isComputer (int x,int y);
int showMenu(SDL_Renderer* ren, bool hasSaveGame);
int showSettings(SDL_Renderer* ren, bool& mute);
int showGuide(SDL_Renderer* ren);
int showWin(SDL_Renderer* ren);
int showShop(SDL_Renderer* ren);
int showLogin(SDL_Renderer* ren);

int showPauseMenu(SDL_Renderer* ren, TTF_Font* font, int winSize);

void loadHighScore();
void saveHighScore();

#endif
