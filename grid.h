#ifndef GRID_H
#define GRID_H
#include "pipe.h"
#include "defs.h"

void genGrid();
Pipe& getPipe(int x, int y);
bool out(const SDL_Point& pos);
void flood(const SDL_Point& pos);

#endif
