#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "grid.h"
#include "defs.h"

void playGame(SDL_Window* win, SDL_Renderer* ren,
              SDL_Texture* bg, SDL_Texture* comp, SDL_Texture* serverTex,
              SDL_Texture* pipeTex, SDL_Texture* glassPipeTex, SDL_Texture* cracksTex,
              SDL_Texture* brokenPipeTex, Mix_Music* music, Mix_Chunk* click);

#endif
