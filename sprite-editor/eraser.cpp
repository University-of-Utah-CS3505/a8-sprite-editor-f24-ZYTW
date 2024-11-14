/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: WYNTER KIM
 */
#include "Eraser.h"

void Eraser::useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) {
    if (x >= 0 && x < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[x][y].setColor(Qt::white);
    }
}
