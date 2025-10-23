#include "pipe.h"

void Pipe::rotate() {
    if (isBroken) return;
    if (pipeType==GLASS) {
        rotationCount++;
        if (rotationCount>3) {
            isBroken=true;
           // dirs.clear();
            return;
        }
    }
    for (auto& d : dirs) {
        if (d.x == 0 && d.y == -1) {
            d.x = 1; d.y = 0;
        } else if (d.x == 1 && d.y == 0) {
            d.x = 0; d.y = 1;
        } else if (d.x == 0 && d.y == 1) {
            d.x = -1; d.y = 0;
        } else if (d.x == -1 && d.y == 0) {
            d.x = 0; d.y = -1;
        }
    }
}

bool Pipe::hasDir(SDL_Point d) const {
    for (const auto& dir : dirs) {
        if (dir.x == d.x && dir.y == d.y) return true;
    }
    return false;
}
