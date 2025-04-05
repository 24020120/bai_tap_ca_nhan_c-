#include "graphic.h"
#include "defs.h"

int showMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuBg = IMG_LoadTexture(renderer, "images/background.png");
    SDL_Texture* startBtn = IMG_LoadTexture(renderer, "images/start.png");
    SDL_Texture* guideBtn = IMG_LoadTexture(renderer, "images/guide.png");
    SDL_Texture* settingsBtn = IMG_LoadTexture(renderer, "images/settings.png");
    SDL_Texture* exitBtn = IMG_LoadTexture(renderer, "images/exit.png");

    const int btnWidth = 100;
    const int btnHeight = 100;
    const int margin = 50;
    const int centerX = windowSize / 2;
    const int centerY = windowSize / 2;
    SDL_Rect startRect = {centerX - btnWidth - margin / 2, centerY - btnHeight - margin / 2, btnWidth, btnHeight};
    SDL_Rect guideRect = {centerX - btnWidth - margin / 2, centerY + margin / 2, btnWidth, btnHeight};
    SDL_Rect settingsRect = {centerX + margin / 2, centerY - btnHeight - margin / 2, btnWidth, btnHeight};
    SDL_Rect exitRect = {centerX + margin / 2, centerY + margin / 2, btnWidth, btnHeight};
    SDL_Rect backgroundRect = {0, 0, windowSize, windowSize};

    bool running = true;
    int selected = NONE;

    extern bool musicMuted; // Truy cập biến toàn cục

    while (running) {
        SDL_Event e;
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                selected = EXIT;
                running = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= startRect.x && mx <= startRect.x + startRect.w && my >= startRect.y && my <= startRect.y + startRect.h) {
                    selected = START;
                    running = false;
                } else if (mx >= guideRect.x && mx <= guideRect.x + guideRect.w && my >= guideRect.y && my <= guideRect.y + guideRect.h) {
                    selected = GUIDE;
                } else if (mx >= settingsRect.x && mx <= settingsRect.x + settingsRect.w && my >= settingsRect.y && my <= settingsRect.y + settingsRect.h) {
                    showSettings(renderer, musicMuted); // Gọi menu Settings
                } else if (mx >= exitRect.x && mx <= exitRect.x + exitRect.w && my >= exitRect.y && my <= exitRect.y + exitRect.h) {
                    selected = EXIT;
                    running = false;
                }
            }
        }

        SDL_SetTextureColorMod(startBtn, 255, 255, 255);
        SDL_SetTextureColorMod(guideBtn, 255, 255, 255);
        SDL_SetTextureColorMod(settingsBtn, 255, 255, 255);
        SDL_SetTextureColorMod(exitBtn, 255, 255, 255);

        if (mx >= startRect.x && mx <= startRect.x + startRect.w && my >= startRect.y && my <= startRect.y + startRect.h) {
            SDL_SetTextureColorMod(startBtn, 255, 255, 0);
        } else if (mx >= guideRect.x && mx <= guideRect.x + guideRect.w && my >= guideRect.y && my <= guideRect.y + guideRect.h) {
            SDL_SetTextureColorMod(guideBtn, 255, 255, 0);
        } else if (mx >= settingsRect.x && mx <= settingsRect.x + settingsRect.w && my >= settingsRect.y && my <= settingsRect.y + settingsRect.h) {
            SDL_SetTextureColorMod(settingsBtn, 255, 255, 0);
        } else if (mx >= exitRect.x && mx <= exitRect.x + exitRect.w && my >= exitRect.y && my <= exitRect.y + exitRect.h) {
            SDL_SetTextureColorMod(exitBtn, 255, 255, 0);
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuBg, NULL, &backgroundRect);
        SDL_RenderCopy(renderer, startBtn, NULL, &startRect);
        SDL_RenderCopy(renderer, guideBtn, NULL, &guideRect);
        SDL_RenderCopy(renderer, settingsBtn, NULL, &settingsRect);
        SDL_RenderCopy(renderer, exitBtn, NULL, &exitRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(menuBg);
    SDL_DestroyTexture(startBtn);
    SDL_DestroyTexture(guideBtn);
    SDL_DestroyTexture(settingsBtn);
    SDL_DestroyTexture(exitBtn);

    return selected;
}

pipe& cell(const SDL_Point &v) {
    return grid[v.y][v.x];
}

bool isOut(const SDL_Point &v) {
    if (v.x < 0 || v.x >= N) return true;
    if (v.y < 0 || v.y >= N) return true;
    return false;
}

void generatePuzzle() {
    std::vector<SDL_Point> nodes;
    SDL_Point start = { rand() % N, rand() % N };
    nodes.push_back(start);

    while (!nodes.empty()) {
        int n = rand() % nodes.size();
        SDL_Point v = nodes[n];
        SDL_Point d = DIR[rand() % 4];
        pipe &p = cell(v);
        if (p.dirs.size() == 3) {
            nodes.erase(nodes.begin() + n);
            continue;
        }
        if (p.dirs.size() == 2) {
            if (rand() % 50 != 0) continue;
        }
        bool complete = true;
        for (auto &D : DIR) {
            SDL_Point adj = { v.x + D.x, v.y + D.y };
            if (!isOut(adj) && cell(adj).dirs.empty()) {
                complete = false;
                break;
            }
        }
        if (complete) {
            nodes.erase(nodes.begin() + n);
            continue;
        }
        SDL_Point to = { v.x + d.x, v.y + d.y };
        if (isOut(to)) continue;
        if (!cell(to).dirs.empty()) continue;
        p.dirs.push_back(d);
        cell(to).dirs.push_back({ -d.x, -d.y });
        nodes.push_back(to);
    }
}

int showSettings(SDL_Renderer* renderer, bool& musicMuted) {
    SDL_Texture* settingsBg = IMG_LoadTexture(renderer, "images/background.png");
    SDL_Texture* muteBtn = IMG_LoadTexture(renderer, "images/mute.png");
    SDL_Texture* unmuteBtn = IMG_LoadTexture(renderer, "images/unmute.png");
    SDL_Texture* backBtn = IMG_LoadTexture(renderer, "images/back.png");

    const int btnWidth = 70;
    const int btnHeight = 70;
    const int margin = 50;
    const int centerX = windowSize / 2;
    const int centerY = windowSize / 2;
    SDL_Rect muteRect = {centerX - btnWidth / 2, centerY - btnHeight - margin / 2, btnWidth, btnHeight}; // Căn giữa
    SDL_Rect backRect = {centerX - btnWidth / 2, centerY + margin / 2, btnWidth, btnHeight}; // Căn giữa
    SDL_Rect backgroundRect = {0, 0, windowSize, windowSize};

    bool running = true;
    int selected = NONE;

    while (running) {
        SDL_Event e;
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                selected = EXIT;
                running = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= muteRect.x && mx <= muteRect.x + muteRect.w && my >= muteRect.y && my <= muteRect.y + muteRect.h) {
                    musicMuted = !musicMuted;
                    if (musicMuted) {
                        Mix_VolumeMusic(0);
                    } else {
                        Mix_VolumeMusic(MIX_MAX_VOLUME);
                    }
                } else if (mx >= backRect.x && mx <= backRect.x + backRect.w && my >= backRect.y && my <= backRect.y + backRect.h) {
                    selected = NONE;
                    running = false;
                }
            }
        }

        SDL_SetTextureColorMod(musicMuted ? unmuteBtn : muteBtn, 255, 255, 255);
        SDL_SetTextureColorMod(backBtn, 255, 255, 255);

        if (mx >= muteRect.x && mx <= muteRect.x + muteRect.w && my >= muteRect.y && my <= muteRect.y + muteRect.h) {
            SDL_SetTextureColorMod(musicMuted ? unmuteBtn : muteBtn, 255, 255, 0);
        } else if (mx >= backRect.x && mx <= backRect.x + backRect.w && my >= backRect.y && my <= backRect.y + backRect.h) {
            SDL_SetTextureColorMod(backBtn, 255, 255, 0);
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, settingsBg, NULL, &backgroundRect);
        SDL_RenderCopy(renderer, musicMuted ? unmuteBtn : muteBtn, NULL, &muteRect);
        SDL_RenderCopy(renderer, backBtn, NULL, &backRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(settingsBg);
    SDL_DestroyTexture(muteBtn);
    SDL_DestroyTexture(unmuteBtn);
    SDL_DestroyTexture(backBtn);

    return selected;
}

int showVictoryScreen(SDL_Renderer* renderer) {
    SDL_Texture* victoryBg = IMG_LoadTexture(renderer, "images/background.png");
    SDL_Texture* replayBtn = IMG_LoadTexture(renderer, "images/replay.png");
    SDL_Texture* exitBtn = IMG_LoadTexture(renderer, "images/exit.png");
    SDL_Texture* gameover = IMG_LoadTexture(renderer, "images/TimeUp.png");
    const int btnWidth = 70;
    const int btnHeight = 70;
    const int margin = 50;
    const int centerX = windowSize / 2;
    const int centerY = windowSize / 2;
    SDL_Rect gameoverRect = {centerX - 100,centerY - 200,200,100};
    SDL_Rect replayRect = {centerX - btnWidth - margin / 2,centerY - btnHeight / 2 + 50,btnWidth, btnHeight};
    SDL_Rect exitRect = {centerX + margin / 2,centerY - btnHeight / 2 + 50,btnWidth, btnHeight};
    SDL_Rect backgroundRect = {0, 0, windowSize, windowSize};
    bool running = true;
    int selected = NONE;
    while (running) {
        SDL_Event e;
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                selected = EXIT;
                running = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= replayRect.x && mx <= replayRect.x + replayRect.w && my >= replayRect.y && my <= replayRect.y + replayRect.h) {
                    selected = REPLAY;
                    running = false;
                } else if (mx >= exitRect.x && mx <= exitRect.x + exitRect.w && my >= exitRect.y && my <= exitRect.y + exitRect.h) {
                    selected = EXIT;
                    running = false;
                }
            }
        }

        SDL_SetTextureColorMod(replayBtn, 255, 255, 255);
        SDL_SetTextureColorMod(exitBtn, 255, 255, 255);

        if (mx >= replayRect.x && mx <= replayRect.x + replayRect.w && my >= replayRect.y && my <= replayRect.y + replayRect.h) {
            SDL_SetTextureColorMod(replayBtn, 255, 255, 0);
        } else if (mx >= exitRect.x && mx <= exitRect.x + exitRect.w && my >= exitRect.y && my <= exitRect.y + exitRect.h) {
            SDL_SetTextureColorMod(exitBtn, 255, 255, 0);
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, victoryBg, NULL, &backgroundRect);
        SDL_RenderCopy(renderer, gameover, NULL, &gameoverRect); // Hiển thị texture gameover
        SDL_RenderCopy(renderer, replayBtn, NULL, &replayRect);
        SDL_RenderCopy(renderer, exitBtn, NULL, &exitRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(victoryBg);
    SDL_DestroyTexture(replayBtn);
    SDL_DestroyTexture(exitBtn);
    SDL_DestroyTexture(gameover);

    return selected;
}

void drop(const SDL_Point &v) {
    pipe &p = cell(v);
    if (p.on) return;
    p.on = true;

    for (auto &d : DIR) {
        SDL_Point to = {v.x + d.x, v.y + d.y};
        if (!isOut(to)) {
            if (p.isConnect(d) && cell(to).isConnect({-d.x, -d.y})) {
                drop(to);
            }
        }
    }
}
