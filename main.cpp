#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdio>
#include "pipe.h"
#include "grid.h"
#include "menu.h"
#include "game.h"
#include "highscore.h"
#include "savegame.h"

bool mute = false;
int gridSize = 6;
int winSize = gridSize * TS + 2 * OFFSET.x;
int rounds = 1;
int score = 0;
std::vector<std::vector<Pipe>> grid;
int itemRemoveComputer = 1;
int itemFixGlass = 1;
int itemAddTime = 1;

extern int highScore;


int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(nullptr)));
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << Mix_GetError() << std::endl;
        return 1;
    }
    SDL_Window* win = SDL_CreateWindow("The Pipe Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winSize, winSize, SDL_WINDOW_SHOWN);
    if (!win) return 1;
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        SDL_DestroyWindow(win);
        Mix_CloseAudio();
        return 1;
    }

    loadHighScore();


    GameState loadedState;
    bool hasSaveGame = loadGame(loadedState, "savegame.json");
    bool loadFromSave = false;


    int choice = showMenu(ren, hasSaveGame);

    if (choice == START) {

        gridSize = 6;
        rounds = 1;
        score = 0;
        mute = false;
        loadFromSave = false;
        itemRemoveComputer = 1;
        itemFixGlass = 1;
        itemAddTime = 1;
        if (hasSaveGame) {
            std::remove("savegame.json");
        }
    } else if (choice == CONTINUE_GAME) {

        grid = loadedState.grid;
        score = loadedState.score;
        mute = loadedState.mute;
        rounds = loadedState.rounds;
        gridSize = loadedState.gridSize;
        itemRemoveComputer = loadedState.itemRemoveComputer;
        itemFixGlass = loadedState.itemFixGlass;
        itemAddTime = loadedState.itemAddTime;

        winSize = gridSize * TS + 2 * OFFSET.x;
        SDL_SetWindowSize(win, winSize, winSize);

        loadFromSave = true;
    }
    else if (choice == EXIT) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        Mix_CloseAudio();
        SDL_Quit();
        return 0;
    }


    SDL_Texture* bg = IMG_LoadTexture(ren, "images/background.png");
    SDL_Texture* comp = IMG_LoadTexture(ren, "images/comp.png");
    SDL_Texture* serverTex = IMG_LoadTexture(ren, "images/server.png");
    SDL_Texture* pipeTex = IMG_LoadTexture(ren, "images/pipes.png");
    SDL_Texture* glassPipeTex = IMG_LoadTexture(ren, "images/glass_pipes.png");
    SDL_Texture* cracksTex = IMG_LoadTexture(ren, "images/cracks.png");
    SDL_Texture* brokenPipeTex = IMG_LoadTexture(ren, "images/broken_pipe.png");
    Mix_Music* music = Mix_LoadMUS("mix/music.mp3");
    Mix_Chunk* click = Mix_LoadWAV("mix/click.wav");


    playGame(win, ren, bg, comp, serverTex, pipeTex, glassPipeTex, cracksTex, brokenPipeTex, music, click,
             loadFromSave, loadedState);


    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(comp);
    SDL_DestroyTexture(serverTex);
    SDL_DestroyTexture(pipeTex);
    SDL_DestroyTexture(glassPipeTex);
    SDL_DestroyTexture(cracksTex);
    SDL_DestroyTexture(brokenPipeTex);
    if (music) Mix_FreeMusic(music);
    if (click) Mix_FreeChunk(click);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
