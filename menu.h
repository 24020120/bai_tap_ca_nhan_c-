#ifndef MENU_H
#define MENU_H
#include "defs.h"
#include <SDL.h>
extern int score;
extern int highScore;
int showMenu(SDL_Renderer* ren);
int showSettings(SDL_Renderer* ren, bool& mute);
int showGuide(SDL_Renderer* ren);
int showWin(SDL_Renderer* ren);
int showShop(SDL_Renderer* ren);
int showLogin(SDL_Renderer* ren);
#endif
