#include"graphic.h"
#include "defs.h"
int showMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuBg = IMG_LoadTexture(renderer, "images/background.png");
    SDL_Texture* startBtn = IMG_LoadTexture(renderer, "images/start.png");
    SDL_Texture* guideBtn = IMG_LoadTexture(renderer, "images/guide.png");
    SDL_Texture* settingsBtn = IMG_LoadTexture(renderer, "images/settings.png");
    SDL_Texture* exitBtn = IMG_LoadTexture(renderer, "images/exit.png");
SDL_Rect startRect = { 250, 100, 260, 260 };
SDL_Rect guideRect = { 250, 400, 260, 260 };
SDL_Rect settingsRect = { 600, 100, 230, 230 };
SDL_Rect backgroundRect = { 0, 0, 800, 600 };
SDL_Rect exitRect = { 600, 400, 240, 240 };
    bool running = true;
    int selected = NONE;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                selected = EXIT;
                running = false;}
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                if (mx >= startRect.x && mx <= startRect.x + startRect.w &&my >= startRect.y && my <= startRect.y + startRect.h) {
                    selected = START;
                    running = false;
                } else if (mx >= guideRect.x && mx <= guideRect.x + guideRect.w &&my >= guideRect.y && my <= guideRect.y + guideRect.h) {
                    selected = GUIDE;
                } else if (mx >= settingsRect.x && mx <= settingsRect.x + settingsRect.w &&my >= settingsRect.y && my <= settingsRect.y + settingsRect.h) {
                    selected = SETTINGS;
                } else if (mx >= exitRect.x && mx <= exitRect.x + exitRect.w &&my >= exitRect.y && my <= exitRect.y + exitRect.h) {
                    selected = EXIT;
                    running = false;
                }}
        }SDL_RenderClear(renderer);
SDL_RenderCopy(renderer, menuBg, NULL, &backgroundRect);
SDL_RenderCopy(renderer, startBtn, NULL, &startRect);
SDL_RenderCopy(renderer, guideBtn, NULL, &guideRect);
SDL_RenderCopy(renderer, settingsBtn, NULL, &settingsRect);
SDL_RenderCopy(renderer, exitBtn, NULL, &exitRect);
        SDL_RenderPresent(renderer);}
    SDL_DestroyTexture(menuBg);
    SDL_DestroyTexture(startBtn);
    SDL_DestroyTexture(guideBtn);
    SDL_DestroyTexture(settingsBtn);
    SDL_DestroyTexture(exitBtn);
    return selected;
}
pipe grid[N][N];
pipe& cell(const SDL_Point &v) {
     return grid[v.y][v.x]; }
bool isOut(const SDL_Point &v)
{
    if(v.x<0||v.x>=N)
        return true;
    if(v.y<0||v.y>=N)
     return true;
    return false;
}
void generatePuzzle(){
    std::vector<SDL_Point> nodes;
    {   SDL_Point start = { rand() % N, rand() % N };
        nodes.push_back(start);
    }
    while(!nodes.empty())
    {int n = rand() % nodes.size();
        SDL_Point v = nodes[n];
        SDL_Point d = DIR[rand() % 4];
        pipe &p = cell(v);
        if(p.dirs.size() == 3)
        {
            nodes.erase(nodes.begin() + n);
            continue;}
        if(p.dirs.size() == 2)
        {
            if(rand() % 50 != 0)
                continue;}
        bool complete = true;
        for(auto &D : DIR)
        {
            SDL_Point adj = { v.x + D.x, v.y + D.y };
            if(!isOut(adj) && cell(adj).dirs.empty())
            {
                complete = false;
                break;}}
        if(complete)
        {
            nodes.erase(nodes.begin() + n);
            continue;
        }
        SDL_Point to = { v.x + d.x, v.y + d.y };
        if(isOut(to)) continue;
        if(!cell(to).dirs.empty()) continue;
        p.dirs.push_back(d);
        cell(to).dirs.push_back( { -d.x, -d.y } );
        nodes.push_back(to);
    }
}
void drop(const SDL_Point &v)
{pipe &p = cell(v);
    if(p.on) return;
    p.on = true;

    for (auto &d : DIR)
    {
        SDL_Point to = {v.x+d.x,v.y+d.y};
        if(!isOut(to))
        {
            if(p.isConnect(d) && cell(to).isConnect({-d.x, -d.y}))
            {
                drop(to);}}
    }}

