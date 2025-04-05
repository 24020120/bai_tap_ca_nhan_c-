#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "graphic.h"
#include "defs.h"
bool musicMuted = false;
int N = 6;
int windowSize = N * ts + 2 * offset.x;
int roundsCompleted = 1;
std::vector<std::vector<pipe>> grid;
int main(int, char**) {
    srand((unsigned)time(nullptr));

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout<< Mix_GetError() << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("The Pipe Puzzle!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize, windowSize, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    int choice = showMenu(renderer);
    if (choice == EXIT) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
    SDL_Texture* tBackground = IMG_LoadTexture(renderer, "images/background.png");
    SDL_Texture* tComp = IMG_LoadTexture(renderer, "images/comp.png");
    SDL_Texture* tServer = IMG_LoadTexture(renderer, "images/server.png");
    SDL_Texture* tPipe = IMG_LoadTexture(renderer, "images/pipes.png");
    Mix_Music* music = Mix_LoadMUS("mix/music.mp3");
    Mix_Chunk* clickSound = Mix_LoadWAV("mix/click.wav");
    if (music) {
        Mix_PlayMusic(music, -1);
        if (musicMuted) Mix_VolumeMusic(0);
        else Mix_VolumeMusic(MIX_MAX_VOLUME);
    }
    SDL_Delay(2000);
    bool gameRunning = true;
    while (gameRunning) {
        windowSize = N * ts + 2 * offset.x;
        SDL_SetWindowSize(window, windowSize, windowSize);

        grid.resize(N, std::vector<pipe>(N));
        generatePuzzle();
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                pipe &p = grid[j][i];
                for (int n = 4; n > 0; n--) {
                    std::string s;
                    for (auto &d : DIR) {
                        s += (p.isConnect(d) ? '1' : '0');
                    }
                    if (s == "0011" || s == "0111" || s == "0101" || s == "0010") {
                        p.orientation = n;
                    }
                    p.rotate();
                }
                int randomRotations = rand() % 4;
                for (int r = 0; r < randomRotations; r++) {
                    p.orientation++;
                    p.rotate();
                }
            }
        }

        SDL_Point servPos = {0, 0};
        do {
            servPos.x = rand() % N;
            servPos.y = rand() % N;
        } while (cell(servPos).dirs.size() == 1);
        drop(servPos);

        bool running = true;
        bool won = false;
        while (running) {
            bool allConnected = true;
            for (int j = 0; j < N; j++) {
                for (int i = 0; i < N; i++) {
                    if (grid[j][i].dirs.size() > 0 && !grid[j][i].on) {
                        allConnected = false;
                        break;
                    }
                }
            }

            if (allConnected) {
                won = true;
                running = false;
            }

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                    gameRunning = false;
                    break;
                } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    mx = mx + ts / 2 - offset.x;
                    my = my + ts / 2 - offset.y;
                    int gx = mx / ts;
                    int gy = my / ts;
                    SDL_Point pos = {gx, gy};
                    if (!isOut(pos)) {
                            if (clickSound) {
                            Mix_PlayChannel(-1, clickSound, 0);
                        }
                        cell(pos).orientation++;
                        cell(pos).rotate();
                        for (int jj = 0; jj < N; jj++) {
                            for (int ii = 0; ii < N; ii++) {
                                grid[jj][ii].on = false;
                            }
                        }
                        drop(servPos);
                    }
                }
            }
            if (!running) break;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Rect bgRect = {0, 0, windowSize, windowSize}; // Background khớp với kích thước cửa sổ
            SDL_RenderCopy(renderer, tBackground, nullptr, &bgRect);

            for (int j = 0; j < N; j++) {
                for (int i = 0; i < N; i++) {
                    pipe &p = grid[j][i];
                    int kind = (int)p.dirs.size();
                    if (kind == 2) {
                        if ((p.dirs[0].x == -p.dirs[1].x) && (p.dirs[0].y == -p.dirs[1].y)) {
                            kind = 0;
                        }
                    }
                    float targetAngle = (p.orientation % 4) * 90.0f;
                    if (p.angle < targetAngle) {
                        p.angle += 5.0f;
                        if (p.angle > targetAngle) p.angle = targetAngle;
                    } else if (p.angle > targetAngle) {
                        p.angle -= 5.0f;
                        if (p.angle < targetAngle) p.angle = targetAngle;
                    }
                    SDL_Rect srcPipe = {ts * kind, 0, ts, ts};
                    SDL_Rect dstPipe = {i * ts + offset.x - 27, j * ts + offset.y - 27, ts, ts};
                    SDL_Point center = {27, 27};
                    SDL_RenderCopyEx(renderer, tPipe, &srcPipe, &dstPipe, p.angle, &center, SDL_FLIP_NONE);
                    if (p.dirs.size() == 1) {
                        SDL_Rect srcComp, dstComp;
                        if (p.on) srcComp = {53, 0, 36, 36};
                        else srcComp = {0, 0, 36, 36};
                        dstComp = {i * ts + offset.x - 18, j * ts + offset.y - 18, 36, 36};
                        SDL_RenderCopy(renderer, tComp,&srcComp, &dstComp);
                    }
                }
            }
            SDL_Rect srcSrv = {0, 0, 40, 40};
            SDL_Rect dstSrv = {servPos.x * ts + offset.x - 20, servPos.y * ts + offset.y - 20, 40, 40};
            SDL_RenderCopy(renderer, tServer, &srcSrv, &dstSrv);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        if (won) {
        roundsCompleted++;
        if (roundsCompleted >= 3) {
            int choice = showVictoryScreen(renderer);
            if (choice == REPLAY) {
                N = 6;
                roundsCompleted = 0;
                grid.clear();
                grid.resize(N, std::vector<pipe>(N));
            } else if (choice == EXIT) {
                gameRunning = false;
            }
        } else {
            N += 1;
            grid.clear();
            grid.resize(N, std::vector<pipe>(N));
        }
    }
    }

    SDL_DestroyTexture(tBackground);
    SDL_DestroyTexture(tComp);
    SDL_DestroyTexture(tServer);
    SDL_DestroyTexture(tPipe);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_FreeChunk(clickSound);
    Mix_CloseAudio();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
