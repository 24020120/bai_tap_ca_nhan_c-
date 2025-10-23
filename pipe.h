#ifndef PIPE_H
#define PIPE_H
#include <vector>
#include <SDL.h>
enum PipeType {
    STEEL,
    GLASS
};
struct Pipe {
    std::vector<SDL_Point> dirs;
    int dir;
    float angle;
    bool on;
    PipeType pipeType;
    int rotationCount;
    bool isBroken;
    Pipe() : dir(0), angle(0.0f), on(false), pipeType(STEEL), rotationCount(0), isBroken(false){}
    void rotate();
    bool hasDir(SDL_Point d) const;
};

#endif
