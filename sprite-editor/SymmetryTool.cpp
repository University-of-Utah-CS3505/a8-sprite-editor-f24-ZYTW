#include "SymmetryTool.h"
#include <QDebug>

SymmetryTool::SymmetryTool(QObject* parent) : QObject(parent) {}

void SymmetryTool::enableSymmetry(bool enabled) {
    symmetryEnabled = enabled;
}

bool SymmetryTool::isSymmetryEnabled() const {
    return symmetryEnabled;
}

void SymmetryTool::drawSymmetry(std::vector<std::vector<Pixel>>& pixels, const QPoint& point, bool horizontal) {
    if (!symmetryEnabled) {
        return;  // Symmetry mode is off
    }

    int canvasWidth = pixels.size();
    int canvasHeight = pixels[0].size();

    int x = point.x();
    int y = point.y();

    if (x >= 0 && x < canvasWidth && y >= 0 && y < canvasHeight) {
        pixels[x][y].setColor(Qt::black);  // Draw original point

        if (horizontal) {
            // Reflect horizontally
            int mirroredY = canvasHeight - 1 - y;
            if (mirroredY >= 0 && mirroredY < canvasHeight) {
                pixels[x][mirroredY].setColor(Qt::black);
            }
        } else {
            // Reflect vertically
            int mirroredX = canvasWidth - 1 - x;
            if (mirroredX >= 0 && mirroredX < canvasWidth) {
                pixels[mirroredX][y].setColor(Qt::black);
            }
        }
    } else {
        qWarning() << "Point is out of canvas bounds!";
    }
}
