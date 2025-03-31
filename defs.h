#ifndef DEFS_H
#define DEFS_H
static const int N=6;
static const int ts=54;
static SDL_Point offset= {65, 55};
static SDL_Point Up= { 0, -1};
static SDL_Point Right= { 1,  0};
static SDL_Point Down = { 0,  1};
static SDL_Point Left= {-1,  0};
static SDL_Point DIR[4]= { Up, Right, Down, Left };
void generatePuzzle();
extern pipe grid[N][N];
pipe& cell(const SDL_Point &v);
bool isOut(const SDL_Point &v);
void drop(const SDL_Point &v);
const int START = 0;
const int GUIDE = 1;
const int SETTINGS = 2;
const int EXIT = 3;
const int NONE = 4;
int showMenu(SDL_Renderer* renderer);
#endif
