#include "menu.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
int showMenu(SDL_Renderer* ren, bool hasSaveGame) {
    SDL_Texture*bg=IMG_LoadTexture(ren,"images/background.png");
    SDL_Texture*btnStart=IMG_LoadTexture(ren,"images/start.png");
    SDL_Texture*btnGuide=IMG_LoadTexture(ren,"images/guide.png");
    SDL_Texture*btnSet=IMG_LoadTexture(ren,"images/settings.png");
    SDL_Texture*btnExit=IMG_LoadTexture(ren,"images/exit.png");
    SDL_Texture*btnShop=IMG_LoadTexture(ren,"images/shop.png");
    SDL_Texture*btnLogin=IMG_LoadTexture(ren,"images/Login.png");


    SDL_Texture* btnContinue = nullptr;
    if (hasSaveGame) {
        btnContinue = IMG_LoadTexture(ren, "images/continue.png");
    }

    const int BW=100;
    const int BH=100;
    const int GAP=50;
    const int cx=winSize/2;
    const int cy=winSize/2;
    const int SMALL_BW=70;
    const int SMALL_BH=70;
    const int MARGIN=30;

    SDL_Rect rStart={cx-BW-GAP/2,cy-BH-GAP/2,BW,BH};
    SDL_Rect rGuide={cx-BW-GAP/2,cy+GAP/2,BW,BH};
    SDL_Rect rSet={cx+GAP/2,cy-BH-GAP/2,BW,BH};
    SDL_Rect rExit={cx+GAP/2,cy+GAP/2,BW,BH};
    SDL_Rect rBg={0,0,winSize,winSize};
    SDL_Rect rShop={MARGIN,winSize-MARGIN-SMALL_BH,SMALL_BW,SMALL_BH};
    SDL_Rect rLogin = {winSize - MARGIN - SMALL_BW, winSize - MARGIN - SMALL_BH, SMALL_BW, SMALL_BH};


    SDL_Rect rContinue = {cx - BW/2,cy+GAP/2+4*GAP/2, BW, BH};
    if (!hasSaveGame) {

        rStart.y -= (BH + GAP) / 4;
        rGuide.y -= (BH + GAP) / 4;
        rSet.y -= (BH + GAP) / 4;
        rExit.y -= (BH + GAP) / 4;
    }


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


                if (hasSaveGame && btnContinue && mx >= rContinue.x && mx <= rContinue.x + rContinue.w && my >= rContinue.y && my <= rContinue.y + rContinue.h) {
                    choice = CONTINUE_GAME;
                    running = false;
                }
                else if (mx >= rStart.x&& mx <= rStart.x + rStart.w && my >=rStart.y &&my <= rStart.y + rStart.h) {
                    choice = START;
                    running = false;
                } else if (mx >= rGuide.x && mx<=rGuide.x + rGuide.w&& my >= rGuide.y&& my <= rGuide.y + rGuide.h) {
                    showGuide(ren);
                } else if (mx >= rSet.x && mx<=rSet.x+rSet.w&& my >= rSet.y && my <= rSet.y + rSet.h) {
                    showSettings(ren, mute);
                } else if (mx >= rExit.x&& mx<=rExit.x+rExit.w&&my >=rExit.y && my <= rExit.y + rExit.h) {
                    choice = EXIT;
                    running = false;
                }  else if (mx >=rShop.x && mx<=rShop.x+rShop.w&&my >=rShop.y && my <= rShop.y + rShop.h) {
                    showShop(ren);
                } else if (mx >= rLogin.x && mx <= rLogin.x + rLogin.w && my >= rLogin.y && my <= rLogin.y + rLogin.h) {
                    showLogin(ren);
                }
            }
        }
        SDL_SetTextureColorMod(btnStart,255,255,255);
        SDL_SetTextureColorMod(btnGuide,255,255,255);
        SDL_SetTextureColorMod(btnSet,255,255,255);
        SDL_SetTextureColorMod(btnExit,255,255,255);
        SDL_SetTextureColorMod(btnShop,255,255,255);
        SDL_SetTextureColorMod(btnLogin, 255, 255, 255);
        if (hasSaveGame && btnContinue) SDL_SetTextureColorMod(btnContinue, 255, 255, 255);

        if (hasSaveGame && btnContinue && mx >= rContinue.x && mx <= rContinue.x + rContinue.w && my >= rContinue.y && my <= rContinue.y + rContinue.h) {
            SDL_SetTextureColorMod(btnContinue, 255, 255, 0);
        }
        else if (mx >= rStart.x && mx <= rStart.x + rStart.w && my >= rStart.y && my <= rStart.y + rStart.h) {
            SDL_SetTextureColorMod(btnStart,255,255,0);
        } else if (mx >= rGuide.x && mx <= rGuide.x + rGuide.w && my >= rGuide.y && my <= rGuide.y + rGuide.h) {
            SDL_SetTextureColorMod(btnGuide,255,255,0);
        } else if (mx >= rSet.x && mx <= rSet.x + rSet.w && my >= rSet.y && my <= rSet.y + rSet.h) {
            SDL_SetTextureColorMod(btnSet, 255,255,0);
        } else if (mx >= rExit.x && mx <= rExit.x + rExit.w && my >= rExit.y && my <= rExit.y + rExit.h) {
            SDL_SetTextureColorMod(btnExit,255,255,0);
        } else if (mx >=rShop.x && mx<=rShop.x+rShop.w&&my >=rShop.y && my <= rShop.y + rShop.h) {
            SDL_SetTextureColorMod(btnShop,255,255,0);
        } else if (mx >= rLogin.x && mx <= rLogin.x + rLogin.w && my >= rLogin.y && my <= rLogin.y + rLogin.h) {
            SDL_SetTextureColorMod(btnLogin, 255, 255, 0);
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bg,nullptr,&rBg);
        SDL_RenderCopy(ren,btnStart,nullptr,&rStart);
        SDL_RenderCopy(ren,btnGuide,nullptr,&rGuide);
        SDL_RenderCopy(ren,btnSet,nullptr,&rSet);
        SDL_RenderCopy(ren,btnExit,nullptr,&rExit);
        SDL_RenderCopy(ren,btnShop,nullptr,&rShop);
        SDL_RenderCopy(ren, btnLogin, nullptr, &rLogin);
        if (hasSaveGame && btnContinue) SDL_RenderCopy(ren, btnContinue, nullptr, &rContinue);

        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnStart);
    SDL_DestroyTexture(btnGuide);
    SDL_DestroyTexture(btnSet);
    SDL_DestroyTexture(btnExit);
    SDL_DestroyTexture(btnShop);
    SDL_DestroyTexture(btnLogin);
    if (btnContinue) SDL_DestroyTexture(btnContinue);
    return choice;
}

int showSettings(SDL_Renderer* ren, bool& mute) {
    SDL_Texture*bg=IMG_LoadTexture(ren,"images/background.png");
    if (!bg) return EXIT;
    SDL_Texture*btnMute=IMG_LoadTexture(ren,"images/mute.png");
    SDL_Texture*btnUnmute=IMG_LoadTexture(ren,"images/unmute.png");
    SDL_Texture*btnBack=IMG_LoadTexture(ren,"images/back.png");
    if (!btnMute||!btnUnmute||!btnBack) {
        SDL_DestroyTexture(bg);
        SDL_DestroyTexture(btnMute);
        SDL_DestroyTexture(btnUnmute);
        SDL_DestroyTexture(btnBack);
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
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 48);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Texture* bg = IMG_LoadTexture(ren, "images/background.png");
    if (!bg) return EXIT;
    SDL_Texture* btnReplay = IMG_LoadTexture(ren, "images/replay.png");
    SDL_Texture* btnExit = IMG_LoadTexture(ren, "images/exit.png");
    SDL_Texture* winTex = IMG_LoadTexture(ren, "images/TimeUp.png");
    if (!btnReplay || !btnExit || !winTex) {
         SDL_DestroyTexture(bg);
         SDL_DestroyTexture(btnReplay);
         SDL_DestroyTexture(btnExit);
         SDL_DestroyTexture(winTex);
        return EXIT;
    }
//    SDL_Rect rBg={0,0,winSize,winSize};
    const int BW = 70;
    const int BH = 70;
    const int GAP = 50;
    const int cx = winSize / 2;
    const int cy = winSize / 2;
    SDL_Rect rWin = {cx - 100, cy - 200, 200, 100};
    SDL_Rect rReplay = {cx - BW - GAP / 2, cy - BH / 2 + 50, BW, BH};
    SDL_Rect rExit = {cx + GAP / 2, cy - BH / 2 + 50, BW, BH};
    SDL_Rect rBg = {0, 0, winSize, winSize};
    int textY = cy - 150;
    int textX = cx - 150;
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
        //extern int score;
        std::string highScoreText = "High Score: " + std::to_string(highScore);
        SDL_Surface* highSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), yellow);
        if (highSurface) {
            SDL_Texture* highTexture = SDL_CreateTextureFromSurface(ren, highSurface);
            if (highTexture) {
                SDL_Rect highRect = {textX, textY + 80, highSurface->w, highSurface->h};
                SDL_RenderCopy(ren, highTexture, nullptr, &highRect);
                SDL_DestroyTexture(highTexture);
            }
            SDL_FreeSurface(highSurface);
        }
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        if (scoreSurface) {
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(ren, scoreSurface);
            if (scoreTexture) {
                SDL_Rect scoreRect = {textX, textY + 40, scoreSurface->w, scoreSurface->h};
                SDL_RenderCopy(ren, scoreTexture, nullptr, &scoreRect);
                SDL_DestroyTexture(scoreTexture);
            }
            SDL_FreeSurface(scoreSurface);
        }
        SDL_RenderCopy(ren, winTex, nullptr, &rWin);
        SDL_RenderCopy(ren, btnReplay, nullptr, &rReplay);
        SDL_RenderCopy(ren, btnExit, nullptr, &rExit);
        SDL_RenderPresent(ren);
        SDL_RenderCopy(ren, bg, nullptr, &rBg);
    }
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnReplay);
    SDL_DestroyTexture(btnExit);
    SDL_DestroyTexture(winTex);
    return choice;
}

int showShop(SDL_Renderer* ren) {
    SDL_Texture* bg=IMG_LoadTexture(ren,"images/background.png");
    SDL_Texture* btnBack=IMG_LoadTexture(ren,"images/back.png");
   // TTF_Font* font = TTF_OpenFont("font/arial.ttf", 40);
    const int BW=70;
    const int BH=50;
    const int cx=winSize/2;
    const int cy=winSize/2;
    SDL_Rect rBack={cx-BW/2,cy+150,BW,BH};
    SDL_Rect rBg={0,0,winSize,winSize};
    bool running=true;
    int choice=NONE;
    while (running) {
        SDL_Event e;
        int mx,my;
        SDL_GetMouseState(&mx,&my);
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT) {
                choice=EXIT;
                running=false;
            } else if (e.type==SDL_MOUSEBUTTONDOWN&&e.button.button==SDL_BUTTON_LEFT) {
                mx=e.button.x;
                my=e.button.y;
                if (mx >= rBack.x && mx <= rBack.x + rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
                    choice=NONE;
                    running=false;
                }
            }
        }
        SDL_SetTextureColorMod(btnBack,255,255,255);
        if (mx >= rBack.x && mx <= rBack.x +rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
            SDL_SetTextureColorMod(btnBack,255,255,0);
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bg,nullptr,&rBg);
        SDL_RenderCopy(ren,btnBack,nullptr,&rBack);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnBack);
    return choice;
}
int showLogin(SDL_Renderer* ren) {
    SDL_Texture* bg = IMG_LoadTexture(ren, "images/background.png");
    if (!bg) return NONE;
    SDL_Texture* btnBack = IMG_LoadTexture(ren, "images/back.png");
    SDL_Texture* btnNext = IMG_LoadTexture(ren, "images/next.png");
        SDL_DestroyTexture(bg);
        if (btnBack) SDL_DestroyTexture(btnBack);
        if (btnNext) SDL_DestroyTexture(btnNext);
        return NONE;


    const int BW = 70;
    const int BH = 70;
    const int GAP = 50;
    const int cx = winSize / 2;
    const int cy = winSize / 2;
    SDL_Rect rBack = {cx - BW - GAP / 2, cy + GAP / 2, BW, BH};
    SDL_Rect rNext = {cx + GAP / 2, cy + GAP / 2, BW, BH};
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
                } else if (mx >= rNext.x && mx <= rNext.x + rNext.w && my >= rNext.y && my <= rNext.y + rNext.h) {

                    choice = LOGIN;
                    running = false;
                }
            }
        }
        SDL_SetTextureColorMod(btnBack, 255, 255, 255);
        SDL_SetTextureColorMod(btnNext, 255, 255, 255);
        if (mx >= rBack.x && mx <= rBack.x + rBack.w && my >= rBack.y && my <= rBack.y + rBack.h) {
            SDL_SetTextureColorMod(btnBack, 255, 255, 0);
        } else if (mx >= rNext.x && mx <= rNext.x + rNext.w && my >= rNext.y && my <= rNext.y + rNext.h) {
            SDL_SetTextureColorMod(btnNext, 255, 255, 0);
        }

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, bg, nullptr, &rBg);
        SDL_RenderCopy(ren, btnBack, nullptr, &rBack);
        SDL_RenderCopy(ren, btnNext, nullptr, &rNext);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(btnBack);
    SDL_DestroyTexture(btnNext);
    return choice;
}
void renderText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color, bool center = true) {
    if (!font) return;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    if (center) {
        rect.x = x - surface->w / 2;
        rect.y = y - surface->h / 2;
    }
    SDL_RenderCopy(ren, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

int showPauseMenu(SDL_Renderer* ren, TTF_Font* font, int winSize) {
    SDL_Texture*bg=IMG_LoadTexture(ren,"images/background.png");
    SDL_Surface* overlaySurface = SDL_CreateRGBSurface(0, winSize, winSize, 32, 0, 0, 0, 0);
    SDL_FillRect(overlaySurface, NULL, SDL_MapRGBA(overlaySurface->format, 0, 0, 0, 150));
    SDL_Texture* overlayTex = SDL_CreateTextureFromSurface(ren, overlaySurface);
    SDL_FreeSurface(overlaySurface);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Rect rBg={0,0,winSize,winSize};
    const int BW = 200;
    const int BH = 70;
    const int GAP = 50;
    const int cx = winSize / 2;
    const int cy = winSize / 2;

    SDL_Rect rResume = {cx - BW / 2, cy - BH - GAP / 2, BW, BH};
    SDL_Rect rExit = {cx - BW / 2, cy + GAP / 2, BW, BH};

    bool running = true;
    int choice = NONE;

    while (running) {
        SDL_Event e;
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choice = EXIT_FROM_PAUSE;
                running = false;
            }
             else if (e.type == SDL_KEYDOWN) {
                 if (e.key.keysym.sym == SDLK_ESCAPE) {
                    choice = CONTINUE_GAME;
                    running = false;
                 }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                if (mx >= rResume.x && mx <= rResume.x + rResume.w && my >= rResume.y && my <= rResume.y + rResume.h) {
                    choice = CONTINUE_GAME;
                    running = false;
                } else if (mx >= rExit.x && mx <= rExit.x + rExit.w && my >= rExit.y && my <= rExit.y + rExit.h) {
                    choice = EXIT_FROM_PAUSE;
                    running = false;
                }
            }
        }


        SDL_RenderCopy(ren, overlayTex, NULL, NULL);


        bool hoverResume = (mx >= rResume.x && mx <= rResume.x + rResume.w && my >= rResume.y && my <= rResume.y + rResume.h);
        bool hoverExit = (mx >= rExit.x && mx <= rExit.x + rExit.w && my >= rExit.y && my <= rExit.y + rExit.h);
        SDL_RenderCopy(ren,bg,nullptr,&rBg);
        SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
        SDL_RenderFillRect(ren, &rResume);
        SDL_RenderFillRect(ren, &rExit);

        renderText(ren, font, "Resume", rResume.x + BW / 2, rResume.y + BH / 2, hoverResume ? yellow : white, true);
        renderText(ren, font, "Save & Exit", rExit.x + BW / 2, rExit.y + BH / 2, hoverExit ? yellow : white, true);


        renderText(ren, font, "PAUSED", cx, cy - 180, white, true);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(overlayTex);
    return choice;
}
/*
int shownext(SDL_Renderer* rea) {
    SDL_Texture* bg=IMG_LoadTexture("images/backgroundNext.png");
    SDL_Texture* btnback=IMG_LoadTexture("images/back.png");
    SDL_Texture* btnext=IMG_LoadTexture("images/next")

}*/
