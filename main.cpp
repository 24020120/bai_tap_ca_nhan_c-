#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "graphic.h"
#include "defs.h"
bool mute=false;
int gridSize=6;
int winSize=gridSize*TS+2*OFFSET.x;
int rounds=1;
std::vector<std::vector<Pipe>> grid;
int main(int argc,char* argv[]) {
    srand(static_cast<unsigned>(time(nullptr)));
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout<<Mix_GetError()<<std::endl;
        return 1;
    }
    SDL_Window*win=SDL_CreateWindow("The Pipe Puzzle",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,winSize,winSize,SDL_WINDOW_SHOWN);
    SDL_Renderer*ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
/*
    if (TTF_Init() == -1) {
    std::cout << "Loi khoi tạo SDL_ttf\n";
    return -1;
}

    if (!font) {
    std::cout << "Khong the load font!\n";
    return -1;
}
*/TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    int choice=showMenu(ren);
    if (choice==EXIT){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }
     SDL_Texture*bg=IMG_LoadTexture(ren,"images/background.png");
     SDL_Texture*comp=IMG_LoadTexture(ren,"images/comp.png");
     SDL_Texture*server=IMG_LoadTexture(ren,"images/server.png");
     SDL_Texture*pipeTex=IMG_LoadTexture(ren,"images/pipes.png");
     Mix_Music*music=Mix_LoadMUS("mix/music.mp3");
     Mix_Chunk*click=Mix_LoadWAV("mix/click.wav");
    if(music){
        Mix_PlayMusic(music,-1);
        Mix_VolumeMusic(mute ? 0 :MIX_MAX_VOLUME);
    }
    SDL_Delay(2000);
    bool playing = true;
    while (playing){
        winSize=gridSize*TS+2*OFFSET.x;
        SDL_SetWindowSize(win,winSize,winSize);
        grid.resize(gridSize,std::vector<Pipe>(gridSize));
        genGrid();
       /**/
     //   const int TIME_LIMIT=60000;
       // Uint32 startTime=SDL_GetTicks();
      /**/

        for (int y=0;y<gridSize;y++) {
            for (int x=0;x<gridSize;x++) {
                Pipe& p=getPipe(x,y);
                for(int n=4;n>0;n--) {
                    std::string dirStr;
                    for(auto& d:DIR) {
                        dirStr+=(p.hasDir(d) ? '1': '0');
                    }
                    if (dirStr=="0011"||dirStr=="0111"||dirStr== "0101"||dirStr == "0010") {
                        p.dir=n;
                    }
                    p.rotate();
                }
                int randRot=rand()%4;
                for (int r=0;r<randRot;r++) {
                    p.dir++;
                    p.rotate();
                }
            }
        }
        SDL_Point serverPos={0, 0};
        do {
            serverPos.x=rand()%gridSize;
            serverPos.y=rand()%gridSize;
        } while (getPipe(serverPos.x,serverPos.y).dirs.size()==1);
        flood(serverPos);
        bool running =true;
        bool win =false;
        while(running) {
            bool allConnected=true;
            for (int y =0;y<gridSize;y++) {
                for (int x=0;x<gridSize;x++) {
                    if (grid[y][x].dirs.size()>0&&!grid[y][x].on) {
                        allConnected=false;
                        break;
                    }
                }
            }
            if (allConnected) {
                win=true;
                running=false;
            }
            SDL_Event e;
            while(SDL_PollEvent(&e)) {
                if(e.type==SDL_QUIT) {
                    running=false;
                    playing=false;
                } else if (e.type==SDL_MOUSEBUTTONDOWN&&e.button.button==SDL_BUTTON_LEFT) {
                    int mx=e.button.x+TS/2-OFFSET.x;
                    int my=e.button.y+TS/2-OFFSET.y;
                    int gx=mx/TS;
                    int gy=my/TS;
                    SDL_Point pos={gx,gy};
                    if (!out(pos)){
                        if (click) Mix_PlayChannel(-1,click,0);
                        Pipe& p =getPipe(gx,gy);
                        p.dir++;
                        p.rotate();
                        for (int y=0;y<gridSize;y++) {
                            for (int x=0;x<gridSize;x++) {
                                grid[y][x].on=false;
                            }
                        }
                        flood(serverPos);
                    }
                }
            }
            SDL_SetRenderDrawColor(ren,0,0,0,255);
            SDL_RenderClear(ren);
            SDL_Rect rBg={0,0,winSize,winSize};
            SDL_RenderCopy(ren,bg,nullptr,&rBg);
        for (int y=0;y<gridSize;y++) {
          for (int x=0;x<gridSize;x++) {
        Pipe& p=grid[y][x];
        int type = static_cast<int>(p.dirs.size());
        if (type == 2 && p.dirs[0].x == -p.dirs[1].x && p.dirs[0].y == -p.dirs[1].y) {
            type = 0;
        }
        float targetAngle=(p.dir%4)*90.0f;
        float deltaAngle = targetAngle-p.angle;
        while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        while (deltaAngle < -180.0f) deltaAngle += 360.0f;
        const float rotationSpeed = 360.0f;
        float maxDelta = rotationSpeed * (16.0f / 1000.0f);
        if (deltaAngle > maxDelta) deltaAngle = maxDelta;
        if (deltaAngle < -maxDelta) deltaAngle = -maxDelta;
        p.angle += deltaAngle;
        while (p.angle >= 360.0f) p.angle -= 360.0f;
        while(p.angle < 0.0f) p.angle += 360.0f;
        SDL_Rect src = {type * TS, 0, TS, TS};
        SDL_Rect dst = {x * TS + OFFSET.x - 27, y * TS + OFFSET.y - 27, TS, TS};
        SDL_Point center = {27, 27};
        SDL_RenderCopyEx(ren, pipeTex, &src, &dst, p.angle, &center, SDL_FLIP_NONE);
        if(p.dirs.size() == 1) {
            SDL_Rect srcComp = {p.on ? 53 : 0, 0, 36, 36};
            SDL_Rect dstComp = {x * TS + OFFSET.x - 18, y * TS + OFFSET.y - 18, 36, 36};
            SDL_RenderCopy(ren, comp, &srcComp, &dstComp);
        }
    }
}
            SDL_Rect srcServer={0,0,40,40};
            SDL_Rect dstServer={serverPos.x*TS+OFFSET.x-20,serverPos.y*TS+OFFSET.y-20,40,40};
            SDL_RenderCopy(ren,server,&srcServer,&dstServer);
            /*
            Uint32 currentTime=SDL_GetTicks();
            int timeLeft=(TIME_LIMIT-(currentTime-startTime))/1000;
            if (timeLeft<0)timeLeft=0;
            SDL_Color color={255,255,255};
            std::string timeText="Time: "+std::to_string(timeLeft);
            SDL_Surface* surface=TTF_RenderText_Solid(font, timeText.c_str(), color);
            SDL_Texture* texture=SDL_CreateTextureFromSurface(ren, surface);
            SDL_Rect dstRect={20,20,surface->w,surface->h};
            SDL_RenderCopy(ren,texture,NULL,&dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            if (timeLeft <= 0) {
                SDL_Delay(1500);
                running = false;}

            */
            SDL_RenderPresent(ren);
            SDL_Delay(16);
        }
        if(win) {
            rounds++;
            if (rounds>=3) {
                int result=showWin(ren);
                if (result==REPLAY) {
                    gridSize=6;
                    rounds=0;
                    grid.clear();
                    grid.resize(gridSize, std::vector<Pipe>(gridSize));
                } else if(result==EXIT) {
                    playing=false;
                }
            } else {
                gridSize++;
                grid.clear();
                grid.resize(gridSize, std::vector<Pipe>(gridSize));
            }
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(comp);
    SDL_DestroyTexture(server);
    SDL_DestroyTexture(pipeTex);
    Mix_FreeMusic(music);
    Mix_FreeChunk(click);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
