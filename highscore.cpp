#include "highscore.h"
#include <fstream>
#include <iostream>
int highScore = 0;
void loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
        std::cout << "Loaded high score: " << highScore << std::endl;
    } else {
        highScore = 0;
    }
}
void saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
        std::cout << "Saved high score: " << highScore << std::endl;
    }
}
