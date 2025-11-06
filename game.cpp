#include "game.h"
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <fstream>
#include <cstdio>
#include "highscore.h"
#include "savegame.h"

extern int rounds;
extern std::vector<std::vector<Pipe>> grid;
extern bool mute;
extern int score;
extern int highScore;

extern int itemRemoveComputer;
extern int itemFixGlass;
extern int itemAddTime;

const int MAX_TIME = 60;

void renderText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color, bool center = true);

void playGame(SDL_Window* win, SDL_Renderer* ren,
              SDL_Texture* bg, SDL_Texture* comp, SDL_Texture* serverTex,
              SDL_Texture* pipeTex, SDL_Texture* glassPipeTex, SDL_Texture* cracksTex,
              SDL_Texture* brokenPipeTex, Mix_Music* music, Mix_Chunk* click,
              bool loadFromSave, const GameState& loadedState) {

    if (music) {
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(mute ? 0 : MIX_MAX_VOLUME);
    }

    bool playing = true;

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 32);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color activeColor = {0, 255, 0, 255};
    SDL_Point serverPos;
    Uint32 startTime;
    int remainingTime = MAX_TIME;
    bool localLoadFromSave = loadFromSave;

    int activeItem = NONE;

    if (localLoadFromSave) {
        serverPos = loadedState.serverPos;
        remainingTime = loadedState.timeRemaining;

        itemRemoveComputer = loadedState.itemRemoveComputer;
        itemFixGlass = loadedState.itemFixGlass;
        itemAddTime = loadedState.itemAddTime;

        std::remove("savegame.json");
    } else {
        SDL_Delay(2000);
    }


    while (playing) {
        int winSize = gridSize * TS + 2 * OFFSET.x;
        SDL_SetWindowSize(win, winSize, winSize);


        if (localLoadFromSave) {
            localLoadFromSave = false;
        } else {
            grid.clear();
            grid.resize(gridSize, std::vector<Pipe>(gridSize));
            genGrid();
            for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                    Pipe& p = getPipe(x, y);
                    for (int n = 4; n > 0; n--) {
                        std::string dirStr;
                        for (auto& d : DIR) {
                            dirStr += (p.hasDir(d) ? '1' : '0');
                        }
                        if (dirStr == "0011" || dirStr == "0111" || dirStr == "0101" || dirStr == "0010") {
                            p.dir = n;
                        }
                        p.rotate();
                    }
                    int randRot = rand() % 4;
                    for (int r = 0; r < randRot; r++) {
                        p.dir++;
                        p.rotate();
                    }
                }
            }
            serverPos = {0, 0};
            do {
                serverPos.x = rand() % gridSize;
                serverPos.y = rand() % gridSize;
            } while (getPipe(serverPos.x, serverPos.y).dirs.size() == 1);

            if (rounds > 1) {
                int glassPipesToMake = std::min(rounds, (gridSize * gridSize) / 4);
                int pipesMade = 0;
                while (pipesMade < glassPipesToMake) {
                    int rx = rand() % gridSize;
                    int ry = rand() % gridSize;
                    if ((rx == serverPos.x && ry == serverPos.y) || getPipe(rx, ry).dirs.empty()) {
                        continue;
                    }
                    Pipe& p = getPipe(rx, ry);
                    if (p.pipeType == STEEL) {
                        p.pipeType = GLASS;
                        p.rotationCount = 0;
                        p.isBroken = false;
                        pipesMade++;
                    }
                }
            }
            remainingTime = MAX_TIME;
        }


        startTime = SDL_GetTicks() - (MAX_TIME - remainingTime) * 1000;

        flood(serverPos);
        bool running = true;
        bool win = false;

        while (running) {
            bool allConnected = true;
            for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                    if (grid[y][x].dirs.size() > 0 && !grid[y][x].on) {
                        allConnected = false;
                        goto next_y;
                    }
                }
                next_y:;
                if (!allConnected) break;
            }
            if (allConnected) {
                win = true;
                running = false;
            }

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                    playing = false;
                }

                else if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        Mix_PauseMusic();

                        int elapsedSeconds = (SDL_GetTicks() - startTime) / 1000;
                        int timeLeft = MAX_TIME - elapsedSeconds;

                        activeItem = NONE;

                        int pauseChoice = showPauseMenu(ren, font, winSize);

                        Mix_ResumeMusic();

                        if (pauseChoice == CONTINUE_GAME) {
                            startTime = SDL_GetTicks() - (MAX_TIME - timeLeft) * 1000;
                        } else if (pauseChoice == EXIT_FROM_PAUSE) {

                            GameState state;
                            state.score = score;
                            state.timeRemaining = timeLeft;
                            state.mute = mute;
                            state.grid = grid;
                            state.serverPos = serverPos;
                            state.rounds = rounds;
                            state.gridSize = gridSize;

                            state.itemRemoveComputer = itemRemoveComputer;
                            state.itemFixGlass = itemFixGlass;
                            state.itemAddTime = itemAddTime;

                            saveGame(state, "savegame.json");

                            running = false;
                            playing = false;
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_1) {
                        if (itemRemoveComputer > 0) {
                            if (click) Mix_PlayChannel(-1, click, 0);
                            activeItem = (activeItem == ITEM_REMOVE_COMPUTER) ? NONE : ITEM_REMOVE_COMPUTER;
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_2) {
                        if (itemFixGlass > 0) {
                            if (click) Mix_PlayChannel(-1, click, 0);
                            activeItem = (activeItem == ITEM_FIX_GLASS) ? NONE : ITEM_FIX_GLASS;
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_3) {
                        if (itemAddTime > 0) {
                            if (click) Mix_PlayChannel(-1, click, 0);
                            itemAddTime--;
                            startTime += 600000;
                            activeItem = NONE;
                        }
                    }
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mx = e.button.x;
                    int my = e.button.y;

                    int gx = (mx + TS / 2 - OFFSET.x) / TS;
                    int gy = (my + TS / 2 - OFFSET.y) / TS;
                    SDL_Point pos = {gx, gy};

                    if (!out(pos)) {
                        Pipe& p = getPipe(gx, gy);


                        if (activeItem == ITEM_REMOVE_COMPUTER) {
                            if (p.dirs.size() == 1) {
                                if (click) Mix_PlayChannel(-1, click, 0);
                                p.dirs.clear();
                                p.on = false;
                                itemRemoveComputer--;
                                activeItem = NONE;

                                for (int yy = 0; yy < gridSize; yy++) {
                                    for (int xx = 0; xx < gridSize; xx++) grid[yy][xx].on = false;
                                }
                                flood(serverPos);
                            }
                        }
                        else if (activeItem == ITEM_FIX_GLASS) {
                            if (p.pipeType == GLASS) {
                                if (click) Mix_PlayChannel(-1, click, 0);
                                p.pipeType = STEEL;
                                p.rotationCount = 0;
                                p.isBroken = false;
                                itemFixGlass--;
                                activeItem = NONE;
                            }
                        }
                        else if (activeItem == NONE) {

                            if (click) Mix_PlayChannel(-1, click, 0);
                            p.dir++;
                            p.rotate();
                            for (int yy = 0; yy < gridSize; yy++) {
                                for (int xx = 0; xx < gridSize; xx++) {
                                    grid[yy][xx].on = false;
                                }
                            }
                            flood(serverPos);
                        }

                    }
                }
            }

            Uint32 currentTime = SDL_GetTicks();
            int elapsedSeconds = (static_cast<Sint64>(currentTime) - static_cast<Sint64>(startTime)) / 1000;
            remainingTime = MAX_TIME - elapsedSeconds;
            if (remainingTime <= 0) {
                running = false;
                win = false;
                 int result = showWin(ren);
                if (result == REPLAY) {
                    gridSize = 6;
                    rounds = 1;
                    score = 0;
                    grid.clear();
                    grid.resize(gridSize, std::vector<Pipe>(gridSize));
                } else if (result == EXIT) {
                    playing = false;
                }
            }

            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderClear(ren);

            SDL_Rect rBg = {0, 0, winSize, winSize};
            SDL_RenderCopy(ren, bg, nullptr, &rBg);


            for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                    Pipe& p = grid[y][x];
                    if (p.isBroken) {
                        continue;
                    }/*
                    if (p.isBroken) {
                        SDL_Rect dst = {x * TS + OFFSET.x - 27, y * TS + OFFSET.y - 27, TS, TS};
                        if (brokenPipeTex) SDL_RenderCopy(ren, brokenPipeTex, nullptr, &dst);
                    }*/
                    int type = static_cast<int>(p.dirs.size());
                    if (type == 2 && p.dirs[0].x == -p.dirs[1].x && p.dirs[0].y == -p.dirs[1].y) {
                        type = 0;
                    }
                    float targetAngle = (p.dir % 4) * 90.0f;
                    float deltaAngle = targetAngle - p.angle;
                    while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
                    while (deltaAngle < -180.0f) deltaAngle += 360.0f;
                    const float rotationSpeed = 360.0f;
                    float maxDelta = rotationSpeed * (16.0f / 1000.0f);
                    if (deltaAngle > maxDelta) deltaAngle = maxDelta;
                    if (deltaAngle < -maxDelta) deltaAngle = -maxDelta;
                    p.angle += deltaAngle;
                    while (p.angle >= 360.0f) p.angle -= 360.0f;
                    while (p.angle < 0.0f) p.angle += 360.0f;

                    SDL_Rect src = {type * TS, 0, TS, TS};
                    SDL_Rect dst = {x * TS + OFFSET.x - 27, y * TS + OFFSET.y - 27, TS, TS};
                    SDL_Point center = {27, 27};

                    if (p.pipeType == STEEL) {
                        // 1. Vẽ ống thép (như cũ)
                        SDL_Rect src = {type * TS, 0, TS, TS};
                        if (pipeTex) {
                            SDL_RenderCopyEx(ren, pipeTex, &src, &dst, p.angle, &center, SDL_FLIP_NONE);
                        }
                    } else {
                        // 2. Xử lý ống thủy tinh
                        if (p.rotationCount == 1 || p.rotationCount == 2) {
                            // Click 1 & 2: Vẽ 'cracks.png'
                            // LUÔN LUÔN lấy ảnh từ HÀNG 0
                            SDL_Rect srcCrack = {type * TS, 0 * TS, TS, TS}; // 0 * TS = Hàng 0
                            if (cracksTex) {
                                SDL_RenderCopyEx(ren, cracksTex, &srcCrack, &dst, p.angle, &center, SDL_FLIP_NONE);
                            }
                        } else {
                            // Bao gồm:
                            // rotationCount == 0 (trạng thái ban đầu)
                            // rotationCount == 3 (click lần 3)
                            // => Sẽ vẽ 'glass_pipes.png'
                            SDL_Rect src = {type * TS, 0, TS, TS};
                            if (glassPipeTex) {
                                SDL_RenderCopyEx(ren, glassPipeTex, &src, &dst, p.angle, &center, SDL_FLIP_NONE);
                            }
                        }
                    }
                    if (p.dirs.size() == 1) {
                        SDL_Rect srcComp = {p.on ? 53 : 0, 0, 36, 36};
                        SDL_Rect dstComp = {x * TS + OFFSET.x - 18, y * TS + OFFSET.y - 18, 36, 36};
                        SDL_RenderCopy(ren, comp, &srcComp, &dstComp);
                    }
                }
            }
            SDL_Rect srcServer = {0, 0, 40, 40};
            SDL_Rect dstServer = {serverPos.x * TS + OFFSET.x - 20, serverPos.y * TS + OFFSET.y - 20, 40, 40};
            SDL_RenderCopy(ren, serverTex, &srcServer, &dstServer);


            if (remainingTime > 0 || win) {
                std::string timeText = "Time: " + std::to_string(std::max(0, remainingTime));
                renderText(ren, font, timeText, 20, 20, white, false);
            }



            std::string scoreText = "Score: " + std::to_string(score);
            SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
            if (scoreSurface) {
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(ren, scoreSurface);
                if (scoreTexture) {
                    SDL_Rect scoreRect = {winSize - 150, 20, scoreSurface->w, scoreSurface->h};
                    SDL_RenderCopy(ren, scoreTexture, nullptr, &scoreRect);
                    SDL_DestroyTexture(scoreTexture);
                }
                SDL_FreeSurface(scoreSurface);
            }

            std::string highScoreText = "High: " + std::to_string(highScore);
            SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), white);
            if (highScoreSurface) {
                SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(ren, highScoreSurface);
                if (highScoreTexture) {
                    SDL_Rect highScoreRect = {winSize - 150, 50, highScoreSurface->w, highScoreSurface->h};
                    SDL_RenderCopy(ren, highScoreTexture, nullptr, &highScoreRect);
                    SDL_DestroyTexture(highScoreTexture);
                }
                SDL_FreeSurface(highScoreSurface);
            }

            SDL_RenderPresent(ren);
            SDL_Delay(16);
        }


        if (win) {
            rounds++;
            score+=50;
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            if (rounds >= 3) {
                activeItem = NONE;
                int result = showWin(ren);
                if (result == REPLAY) {
                    gridSize = 6;
                    rounds = 1;
                    score = 0;
                    grid.clear();
                    grid.resize(gridSize, std::vector<Pipe>(gridSize));
                } else if (result == EXIT) {
                    playing = false;
                }
            } else {
                gridSize++;
            }
        }
    }

    if (font) TTF_CloseFont(font);
    TTF_Quit();
}
