#include "game.h"
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

extern int rounds;
extern std::vector<std::vector<Pipe>> grid;
extern bool mute;

void playGame(SDL_Window* win, SDL_Renderer* ren,
              SDL_Texture* bg, SDL_Texture* comp, SDL_Texture* serverTex,
              SDL_Texture* pipeTex, SDL_Texture* glassPipeTex, SDL_Texture* cracksTex,
              SDL_Texture* brokenPipeTex, Mix_Music* music, Mix_Chunk* click) {
    if (music) {
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(mute ? 0 : MIX_MAX_VOLUME);
    }
    SDL_Delay(2000);

    bool playing = true;
    while (playing) {
        int winSize = gridSize * TS + 2 * OFFSET.x;
        SDL_SetWindowSize(win, winSize, winSize);
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
        SDL_Point serverPos = {0, 0};
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
                } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mx = e.button.x + TS / 2 - OFFSET.x;
                    int my = e.button.y + TS / 2 - OFFSET.y;
                    int gx = mx / TS;
                    int gy = my / TS;
                    SDL_Point pos = {gx, gy};
                    if (!out(pos)) {
                        if (click) Mix_PlayChannel(-1, click, 0);
                        Pipe& p = getPipe(gx, gy);
                        p.dir++;
                        p.rotate();
                        // Reset on
                        for (int yy = 0; yy < gridSize; yy++) {
                            for (int xx = 0; xx < gridSize; xx++) {
                                grid[yy][xx].on = false;
                            }
                        }
                        flood(serverPos);
                    }
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
                        SDL_Rect dst = {x * TS + OFFSET.x - 27, y * TS + OFFSET.y - 27, TS, TS};
                        if (brokenPipeTex) SDL_RenderCopy(ren, brokenPipeTex, nullptr, &dst);
                    }
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

                    SDL_Texture* texToUse = (p.pipeType == GLASS) ? glassPipeTex : pipeTex;
                    if (texToUse) {
                        SDL_RenderCopyEx(ren, texToUse, &src, &dst, p.angle, &center, SDL_FLIP_NONE);
                    }
                    if (p.pipeType == GLASS && p.rotationCount > 0 && p.rotationCount <= 3) {
                        SDL_Rect srcCrack = {(p.rotationCount - 1) * TS, 0, TS, TS};
                        if (cracksTex) {
                            SDL_RenderCopyEx(ren, cracksTex, &srcCrack, &dst, p.angle, &center, SDL_FLIP_NONE);
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
            SDL_RenderPresent(ren);
            SDL_Delay(16);
        }
        if (win) {
            rounds++;
            if (rounds >= 3) {
                int result = showWin(ren);
                if (result == REPLAY) {
                    gridSize = 6;
                    rounds = 1;
                    grid.clear();
                    grid.resize(gridSize, std::vector<Pipe>(gridSize));
                } else if (result == EXIT) {
                    playing = false;
                }
            } else {
                gridSize++;
                grid.clear();
                grid.resize(gridSize, std::vector<Pipe>(gridSize));
            }
        }
    }
}
