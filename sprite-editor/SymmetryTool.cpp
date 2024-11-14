/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: ZHENGXI ZHANG
 */
#include "SymmetryTool.h"

SymmetryTool::SymmetryTool(QObject* parent)
    : Tool(parent) {}

void SymmetryTool::useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) {
    // draw this point
    if (x >= 0 && x < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[x][y].setColor(color);
    }

    // Draw the symmetric point
    int mirroredX = canvasSize - 1 - x;
    if (mirroredX >= 0 && mirroredX < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[mirroredX][y].setColor(color);
    }
}

void SymmetryTool::setCanvasSize(int size) {
    canvasSize = size;
}

void SymmetryTool::setColor(QColor newColor) {
    color = newColor;
}
