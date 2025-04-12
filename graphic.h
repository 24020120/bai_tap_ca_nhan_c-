#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <vector>
#include <SDL.h>
struct Pipe {
    std::vector<SDL_Point> dirs;
    int dir;
    float angle;
    bool on;
    Pipe() : dir(0), angle(0.0f), on(false) {}
    void rotate() {
        for (auto& d:dirs) {
            if(d.x==0&&d.y==-1){
                d.x=1;d.y=0;}
            else if(d.x==1&&d.y==0){
                 d.x=0;d.y=1;}
            else if(d.x==0&&d.y==1){
                d.x=-1;d.y=0; }
            else if(d.x==-1&&d.y==0)
            {
            d.x=0;d.y=-1;}
        }
    }
    bool hasDir(SDL_Point d) const {
        for (const auto&dir:dirs) {
            if (dir.x==d.x&&dir.y==d.y)
                return true;
        }
        return false;
    }
};
extern std::vector<std::vector<Pipe>> grid;
#endif
