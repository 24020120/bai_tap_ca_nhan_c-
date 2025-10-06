#include "game.h"
#include "menu.h"
#include "grid.h"
#include "render.h"
#include "defs.h"
#include <iostream>
#include <vector>
#include <SDL_image.h>
#include <ctime>

bool mute = false;
int winSize = gridSize * TS + 2 * OFFSET.x;
int rounds = 1;

SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;
TTF_Font* font = nullptr;
Mix_Music* music = nullptr;

SDL_Texture* pipeTex = nullptr;
SDL_Texture* comp = nullptr;
SDL_Texture* server = nullptr;
SDL_Texture* bg = nullptr;

bool initGame() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "TTF_Init failed\n";
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mix_OpenAudio failed\n";
        return false;
    }

    win = SDL_CreateWindow("The Pipe Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winSize, winSize, SDL_WINDOW_SHOWN);
    if (!win) return false;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) return false;
    font = TTF_OpenFont("assets/arial.ttf", 24);
    if (!font) {
        std::cout << "Font failed, using system\n";
    }

    pipeTex = IMG_LoadTexture(ren, "assets/pipes.png");
    comp = IMG_LoadTexture(ren, "assets/comp.png");
    server = IMG_LoadTexture(ren, "assets/server.png");
    bg = IMG_LoadTexture(ren, "assets/background.png");
    music = Mix_LoadMUS("assets/music.mp3");
    if (music) {
        if (!mute) Mix_PlayMusic(music, -1);
        else Mix_VolumeMusic(0);
    }

    return true;
}

void runGame() {
    bool quit = false;
    while (!quit) {
        int choice = showMenu(ren, font);

        if (choice == EXIT) quit = true;
        else if (choice == GUIDE)
            showGuide(ren, font);
        else if (choice == SETTINGS) {
            showSettings(ren, mute);
            if (music) {
                Mix_VolumeMusic(mute ? 0 : MIX_MAX_VOLUME);
                if (!mute) Mix_PlayMusic(music, -1);
            }
        } else {  // NEWGAME
            genGrid();
            SDL_Point serverPos = {0, 0};
            grid[0][0].on = true;

            bool running = true;
            while (running) {
                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                        quit = true;
                    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                        int x = (e.button.x - OFFSET.x) / TS;
                        int y = (e.button.y - OFFSET.y) / TS;
                        if (x >= 0 && x < gridSize && y >= 0 && y < gridSize && !(x == 0 && y == 0)) {
                            grid[y][x].rotate();
                            flood(serverPos);
                        }
                    }
                }

                renderGrid(ren, pipeTex, comp, server, bg, serverPos);

                if (isWin()) {
                    showWin(ren, font);  // Sửa: Pass font
                    running = false;
                }
                SDL_Delay(16);
            }
        }
    }
}

void cleanUp() {
    if (music) Mix_FreeMusic(music);
    if (pipeTex) SDL_DestroyTexture(pipeTex);
    if (comp) SDL_DestroyTexture(comp);
    if (server) SDL_DestroyTexture(server);
    if (bg) SDL_DestroyTexture(bg);
    if (font) TTF_CloseFont(font);
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
