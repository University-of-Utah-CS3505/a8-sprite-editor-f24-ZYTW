#include "Eraser.h"

void Eraser::useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) {
    if (x >= 0 && x < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[x][y].setColor(Qt::white);
    }
}
