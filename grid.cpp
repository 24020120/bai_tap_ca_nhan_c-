#include "grid.h"
#include <cstdlib>

Pipe& getPipe(int x, int y) {
    return grid[y][x];
}

bool out(const SDL_Point& pos) {
    return pos.x < 0 || pos.x >= gridSize || pos.y < 0 || pos.y >= gridSize;
}

void genGrid() {
    std::vector<SDL_Point> nodes;
    SDL_Point start = {static_cast<int>(rand()) % gridSize, static_cast<int>(rand()) % gridSize};
    nodes.push_back(start);
    while (!nodes.empty()) {
        int i = rand() % static_cast<int>(nodes.size());
        SDL_Point pos = nodes[i];
        SDL_Point dir = DIR[rand() % 4];
        Pipe& p = getPipe(pos.x, pos.y);
        if (p.dirs.size() == 3) {
            nodes.erase(nodes.begin() + i);
            continue;
        }
        if (p.dirs.size() == 2 && rand() % 50 != 0) {
            continue;
        }
        bool done = true;
        for (const auto& d : DIR) {
            SDL_Point next = {pos.x + d.x, pos.y + d.y};
            if (!out(next) && getPipe(next.x, next.y).dirs.empty()) {
                done = false;
                break;
            }
        }
        if (done) {
            nodes.erase(nodes.begin() + i);
            continue;
        }
        SDL_Point to = {pos.x + dir.x, pos.y + dir.y};
        if (out(to) || !getPipe(to.x, to.y).dirs.empty()) {
            continue;
        }
        p.dirs.push_back(dir);
        getPipe(to.x, to.y).dirs.push_back({-dir.x, -dir.y});
        nodes.push_back(to);
    }
}

void flood(const SDL_Point& pos) {
    Pipe& p = getPipe(pos.x, pos.y);
    if (p.on) return;
    p.on = true;
    for (const auto& d : DIR) {
        SDL_Point next = {pos.x + d.x, pos.y + d.y};
        if (!out(next) && p.hasDir(d) && getPipe(next.x, next.y).hasDir({-d.x, -d.y})) {
            flood(next);
        }
    }
}
