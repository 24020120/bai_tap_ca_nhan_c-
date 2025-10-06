#ifndef PIPE_H
#define PIPE_H
#include <vector>
#include <SDL.h>

struct Pipe {
    std::vector<SDL_Point> dirs;
    int dir;
    float angle;
    bool on;
    Pipe() : dir(0), angle(0.0f), on(false) {}
    void rotate();
    bool hasDir(SDL_Point d) const;
};

#endif
