#include "savegame.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

bool saveGame(const GameState& state, const std::string& filename) {
    json j;
    j["score"] = state.score;
    j["timeRemaining"] = state.timeRemaining;
    j["mute"] = state.mute;
    j["rounds"] = state.rounds;
    j["gridSize"] = state.gridSize;


    j["itemRemoveComputer"] = state.itemRemoveComputer;
    j["itemFixGlass"] = state.itemFixGlass;
    j["itemAddTime"] = state.itemAddTime;


    j["coins"] = state.coins;


    j["serverPos"] = {{"x", state.serverPos.x}, {"y", state.serverPos.y}};



    j["grid"] = json::array();
    for (auto& row : state.grid) {
        json jrow = json::array();
        for (auto& pipe : row) {
            json p;
            p["on"] = pipe.on;
            p["isBroken"] = pipe.isBroken;
            p["pipeType"] = pipe.pipeType;
            p["rotationCount"] = pipe.rotationCount;
            p["dir"] = pipe.dir;

            p["dirs"] = json::array();
            for (auto& d : pipe.dirs) {
                p["dirs"].push_back({{"x", d.x}, {"y", d.y}});
            }

            jrow.push_back(p);
        }
        j["grid"].push_back(jrow);
    }

    std::ofstream file(filename);
    if (!file.is_open()) return false;
    file << j.dump(4);
    return true;
}

bool loadGame(GameState& state, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    json j;
    try {
        file >> j;

        state.score = j["score"];
        state.timeRemaining = j["timeRemaining"];
        state.mute = j["mute"];
        state.rounds = j["rounds"];
        state.gridSize = j["gridSize"];

        state.itemRemoveComputer = j["itemRemoveComputer"];
        state.itemFixGlass = j["itemFixGlass"];
        state.itemAddTime = j["itemAddTime"];


        state.coins = j.value("coins", 0);


        state.serverPos = {j["serverPos"]["x"], j["serverPos"]["y"]};

        state.grid.clear();
        for (auto& jrow : j["grid"]) {
            std::vector<Pipe> row;
            for (auto& jp : jrow) {
                Pipe p;
                p.on = jp["on"];
                p.isBroken = jp["isBroken"];
                p.pipeType = jp["pipeType"];
                p.rotationCount = jp["rotationCount"];
                p.dir = jp["dir"];
                p.angle = (p.dir % 4) * 90.0f;
                p.dirs.clear();
                for (auto& jd : jp["dirs"]) {
                    SDL_Point d = {jd["x"], jd["y"]};
                    p.dirs.push_back(d);
                }
                row.push_back(p);
            }
            state.grid.push_back(row);
        }
    } catch (json::exception& e) {

        file.close();
        return false;
    }

    file.close();
    return true;
}
