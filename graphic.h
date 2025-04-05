#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL_mixer.h>

struct pipe {
    std::vector<SDL_Point> dirs;
    int orientation;
    float angle;
    bool on;
    pipe() : orientation(0), angle(0.0f), on(false) {}
    void rotate() {
        for (auto &d : dirs) {
            if (d.x == 0 && d.y == -1) { d.x = 1; d.y = 0; }
            else if (d.x == 1 && d.y == 0) { d.x = 0; d.y = 1; }
            else if (d.x == 0 && d.y == 1) { d.x = -1; d.y = 0; }
            else if (d.x == -1 && d.y == 0) { d.x = 0; d.y = -1; }
        }
    }
    bool isConnect(SDL_Point dir) {
        for (auto &d : dirs) {
            if (d.x == dir.x && d.y == dir.y) return true;
        }
        return false;
    }
};

extern std::vector<std::vector<pipe>> grid; // Chỉ khai báo, không định nghĩa

#endif
