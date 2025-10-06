#include "menu.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL.h>

int showMenu(SDL_Renderer* ren) {
    SDL_Texture*bg=IMG_LoadTexture(ren,"images/background.png");
    if (!bg) return EXIT;
    SDL_Texture*btnStart=IMG_LoadTexture(ren,"images/start.png");
    SDL_Texture*btnGuide=IMG_LoadTexture(ren,"images/guide.png");
    SDL_Texture*btnSet=IMG_LoadTexture(ren,"images/settings.png");
    SDL_Texture*btnExit=IMG_LoadTexture(ren,"images/exit.png");
    if (!btnStart||!btnGuide||!btnSet||!btnExit) {
        SDL_DestroyTexture(bg); SDL_DestroyTexture(btnStart); SDL_DestroyTexture(btnGuide); SDL_DestroyTexture(btnSet); SDL_DestroyTexture(btnExit);
        return EXIT;
    }
    const int BW=100;
    const int BH=100;
    const int GAP=50;
    const int cx=winSize/2;
    const int cy=winSize/2;
    SDL_Rect rStart={cx-BW-GAP/2,cy-BH-GAP/2,BW,BH};
    SDL_Rect rGuide={cx-BW-GAP/2,cy+GAP/2,BW,BH};
    SDL_Rect rSet={cx+GAP/2,cy-BH-GAP/2,BW,BH};
    SDL_Rect rExit={cx+GAP/2,cy+GAP/2,BW,BH};
    SDL_Rect rBg={0,0,winSize,winSize};
    bool running=true;
    int choice=NONE;
    while(running) {
        SDL_Event e;
        int mx,my;
        SDL_GetMouseState(&mx,&my);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choice = EXIT;
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= rStart.x&& mx <= rStart.x + rStart.w && my >=rStart.y &&my <= rStart.y + rStart.h) {
                    choice = START;
                    running = false;
                } else if (mx >= rGuide.x && mx<=rGuide.x + rGuide.w&& my >= rGuide.y&& my <= rGuide.y + rGuide.h) {
                    showGuide(ren);
                } else if (mx >= rSet.x && mx<=rSet.x+rSet.w&& my >= rSet.y && my <= rSet.y + rSet.h) {
                    showSettings(ren, mute);
                } else if (mx >= rExit.x&& mx<=rExit.x+rExit.w&&my >=rExit.y && my <= rExit.y + rExit.h) {
                    choice = EXIT;
                    running = false;
                }
            }
        }
        SDL_SetTextureColorMod(btnStart,255,255,255);
        SDL_SetTextureColorMod(btnGuide,255,255,255);
        SDL_SetTextureColorMod(btnSet,255,255,255);
        SDL_SetTextureColorMod(btnExit,255,255,255);
        if (mx>=rStart.x&&mx<=rStart.x+rStart.w&&my>=rStart.y&&my<=rStart.y+rStart.h) {
            SDL_SetTextureColorMod(btnStart,255,255,0);
        } else if (mx >= rGuide.x && mx <= rGuide.x + rGuide.w && my >= rGuide.y && my <= rGuide.y + rGuide.h) {
            SDL_SetTextureColorMod(btnGuide,255,255,0);
        } else if (mx >= rSet.x && mx <= rSet.x + rSet.w && my >= rSet.y && my <= rSet.y + rSet.h) {
            SDL_SetTextureColorMod(btnSet, 255,255,0);
        } else if (mx >= rExit.x && mx <= rExit.x + rExit.w && my >= rExit.y && my <= rExit.y + rExit.h) {
            SDL_SetTextureColorMod(btnExit,255,255,0);
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bg,nullptr,&rBg);
        SDL_RenderCopy(ren,btnStart,nullptr,&rStart);
        SDL_RenderCopy(ren,btnGuide,nullptr,&rGuide);
        SDL_RenderCopy(ren,btnSet,nullptr,&rSet);
        SDL_RenderCopy(ren,btnExit,nullptr,&rExit);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnStart);
    SDL_DestroyTexture(btnGuide);
    SDL_DestroyTexture(btnSet);
    SDL_DestroyTexture(btnExit);
    return choice;
}

int showSettings(SDL_Renderer* ren, bool& mute) {
    SDL_Texture*bg=IMG_LoadTexture(ren,"images/background.png");
    if (!bg) return EXIT;
    SDL_Texture*btnMute=IMG_LoadTexture(ren,"images/mute.png");
    SDL_Texture*btnUnmute=IMG_LoadTexture(ren,"images/unmute.png");
    SDL_Texture*btnBack=IMG_LoadTexture(ren,"images/back.png");
    if (!btnMute||!btnUnmute||!btnBack) {
        SDL_DestroyTexture(bg);SDL_DestroyTexture(btnMute); SDL_DestroyTexture(btnUnmute); SDL_DestroyTexture(btnBack);
        return EXIT;
    }
    const int BW=70;
    const int BH=70;
    const int GAP=50;
    const int cx=winSize/2;
    const int cy=winSize/2;
    SDL_Rect rMute={cx-BW/2,cy-BH-GAP/2,BW,BH};
    SDL_Rect rBack={cx-BW/2,cy+GAP/2,BW,BH};
    SDL_Rect rBg={0,0,winSize,winSize};
    bool running=true;
    int choice=NONE;
    while (running) {
        SDL_Event e;
        int mx,my;
        SDL_GetMouseState(&mx,&my);
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT) {
                choice = EXIT;
                running=false;
            } else if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                mx=e.button.x;
                my=e.button.y;
                if (mx>=rMute.x&&mx<=rMute.x+ rMute.w && my >= rMute.y && my <= rMute.y + rMute.h) {
                    mute = !mute;
                    Mix_VolumeMusic(mute ? 0 : MIX_MAX_VOLUME);
                } else if (mx >= rBack.x && mx <= rBack.x + rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
                    choice = NONE;
                    running = false;
                }
            }
        }
        SDL_SetTextureColorMod(mute ? btnUnmute : btnMute, 255, 255, 255);
        SDL_SetTextureColorMod(btnBack, 255, 255, 255);
        if (mx >= rMute.x && mx <= rMute.x + rMute.w && my >= rMute.y && my <= rMute.y + rMute.h) {
            SDL_SetTextureColorMod(mute ? btnUnmute : btnMute, 255, 255, 0);
        } else if (mx >= rBack.x && mx <= rBack.x + rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
            SDL_SetTextureColorMod(btnBack, 255, 255, 0);
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, bg, nullptr, &rBg);
        SDL_RenderCopy(ren, mute ? btnUnmute : btnMute, nullptr, &rMute);
        SDL_RenderCopy(ren, btnBack, nullptr, &rBack);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnMute);
    SDL_DestroyTexture(btnUnmute);
    SDL_DestroyTexture(btnBack);
    return choice;
}

int showGuide(SDL_Renderer* ren) {
    SDL_Texture* bg = IMG_LoadTexture(ren, "images/backgroundGuide.png");
    if (!bg) return EXIT;
    SDL_Texture* btnBack = IMG_LoadTexture(ren, "images/back.png");
    if (!btnBack) {
        SDL_DestroyTexture(bg);
        return EXIT;
    }

    const int BW = 70;
    const int BH = 70;
    const int GAP = 50;
    const int cx = winSize / 2;
    const int cy = winSize / 2;
    SDL_Rect rBack = {cx - BW / 2, cy + GAP / 2 + 100, BW, BH};
    SDL_Rect rBg = {0, 0, winSize, winSize};
    bool running = true;
    int choice = NONE;
    while (running) {
        SDL_Event e;
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choice = EXIT;
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= rBack.x && mx <= rBack.x + rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
                    choice = NONE;
                    running = false;
                }
            }
        }
        SDL_SetTextureColorMod(btnBack, 255, 255, 255);
        if (mx >= rBack.x && mx <= rBack.x + rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
            SDL_SetTextureColorMod(btnBack, 255, 255, 0);
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, bg, nullptr, &rBg);
        SDL_RenderCopy(ren, btnBack, nullptr, &rBack);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnBack);
    return choice;
}

int showWin(SDL_Renderer* ren) {
    SDL_Texture* bg = IMG_LoadTexture(ren, "images/background.png");
    if (!bg) return EXIT;
    SDL_Texture* btnReplay = IMG_LoadTexture(ren, "images/replay.png");
    SDL_Texture* btnExit = IMG_LoadTexture(ren, "images/exit.png");
    SDL_Texture* winTex = IMG_LoadTexture(ren, "images/TimeUp.png");
    if (!btnReplay || !btnExit || !winTex) {
        SDL_DestroyTexture(bg); SDL_DestroyTexture(btnReplay); SDL_DestroyTexture(btnExit); SDL_DestroyTexture(winTex);
        return EXIT;
    }

    const int BW = 70;
    const int BH = 70;
    const int GAP = 50;
    const int cx = winSize / 2;
    const int cy = winSize / 2;
    SDL_Rect rWin = {cx - 100, cy - 200, 200, 100};
    SDL_Rect rReplay = {cx - BW - GAP / 2, cy - BH / 2 + 50, BW, BH};
    SDL_Rect rExit = {cx + GAP / 2, cy - BH / 2 + 50, BW, BH};
    SDL_Rect rBg = {0, 0, winSize, winSize};
    bool running = true;
    int choice = NONE;
    while (running) {
        SDL_Event e;
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choice = EXIT;
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= rReplay.x && mx <= rReplay.x + rReplay.w && my >= rReplay.y && my <= rReplay.y + rReplay.h) {
                    choice = REPLAY;
                    running = false;
                } else if (mx >= rExit.x && mx <= rExit.x + rExit.w && my >= rExit.y && my <= rExit.y + rExit.h) {
                    choice = EXIT;
                    running = false;
                }
            }
        }
        SDL_SetTextureColorMod(btnReplay, 255, 255, 255);
        SDL_SetTextureColorMod(btnExit, 255, 255, 255);
        if (mx >= rReplay.x && mx <= rReplay.x + rReplay.w && my >= rReplay.y && my <= rReplay.y + rReplay.h) {
            SDL_SetTextureColorMod(btnReplay, 255, 255, 0);
        } else if (mx >= rExit.x && mx <= rExit.x + rExit.w && my >= rExit.y && my <= rExit.y + rExit.h) {
            SDL_SetTextureColorMod(btnExit, 255, 255, 0);
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, bg, nullptr, &rBg);
        SDL_RenderCopy(ren, winTex, nullptr, &rWin);
        SDL_RenderCopy(ren, btnReplay, nullptr, &rReplay);
        SDL_RenderCopy(ren, btnExit, nullptr, &rExit);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnReplay);
    SDL_DestroyTexture(btnExit);
    SDL_DestroyTexture(winTex);
    return choice;
}
