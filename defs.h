#ifndef DEFS_H
#define DEFS_H
extern int N; // Kích thước lưới
extern int windowSize; // Kích thước cửa sổ (tính bằng pixel)
static const int ts = 54; // Kích thước ô
static SDL_Point offset = {65, 55};
static SDL_Point Up = {0, -1};
static SDL_Point Right = {1, 0};
static SDL_Point Down = {0, 1};
static SDL_Point Left = {-1, 0};
static SDL_Point DIR[4] = {Up, Right, Down, Left};
void generatePuzzle();
extern std::vector<std::vector<pipe>> grid;
pipe& cell(const SDL_Point &v);
bool isOut(const SDL_Point &v);
void drop(const SDL_Point &v);
const int START = 0;
const int GUIDE = 1;
const int SETTINGS = 2;
const int EXIT = 3;
const int NONE = 4;
const int REPLAY = 5;
int showMenu(SDL_Renderer* renderer);
int showEndScreen(SDL_Renderer* renderer, bool won);
int showSettings(SDL_Renderer* renderer, bool& musicMuted);
int showVictoryScreen(SDL_Renderer* renderer);
extern bool musicMuted;
#endif
