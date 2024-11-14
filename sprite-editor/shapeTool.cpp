/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: ZHENGXI ZHANG
 */
#include "shapeTool.h"

ShapeTool::ShapeTool(QObject* parent, const QString& initialShapeType)
    : Tool(parent), shapeType(initialShapeType) {}

void ShapeTool::setShapeType(const QString& type) {
    shapeType = type;
}

void ShapeTool::startDragging(const QPoint& start) {
    startPoint = start;
    isDragging = true;
}

void ShapeTool::stopDragging() {
    isDragging = false;
}

void ShapeTool::useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) {
    if (isDragging) {
        endPoint = QPoint(x, y);
        drawShape(pixels);  // Draw shape on mouse release
        stopDragging();
    }
}

void ShapeTool::drawShape(std::vector<std::vector<Pixel>>& pixels) {
    int startX = std::min(startPoint.x(), endPoint.x());
    int startY = std::min(startPoint.y(), endPoint.y());
    int endX = std::max(startPoint.x(), endPoint.x());
    int endY = std::max(startPoint.y(), endPoint.y());

    if (shapeType == "Rectangle") {
        for (int x = startX; x <= endX; ++x) {
            pixels[x][startY].setColor(color);  // Top edge
            pixels[x][endY].setColor(color);    // Bottom edge
        }
        for (int y = startY; y <= endY; ++y) {
            pixels[startX][y].setColor(color);  // Left edge
            pixels[endX][y].setColor(color);    // Right edge
        }
    } else if (shapeType == "Ellipse") {
        drawEllipse(startX, startY, endX, endY, pixels);
    } else if (shapeType == "Triangle") {
        drawTriangle(startX, startY, endX, endY, pixels);
    }
}

void ShapeTool::drawEllipse(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels) {
    // Calculate ellipse parameters
    int a = abs((endX - startX) / 2);  // Semi-major axis
    int b = abs((endY - startY) / 2);  // Semi-minor axis
    int centerX = startX + a;
    int centerY = startY + b;

    float dx, dy, d1, d2;
    int x = 0, y = b;

    // 1st D parameter
    d1 = (b * b) - (a * a * b) + (0.25 * a * a);
    dx = 2 * b * b * x;
    dy = 2 * a * a * y;

    // Top to midpoint Region1:
    while (dx < dy) {
        drawSymmetricPixels(centerX, centerY, x, y, pixels);
        if (d1 < 0) {
            x++;
            dx += 2 * b * b;
            d1 += dx + (b * b);
        } else {
            x++;
            y--;
            dx += 2 * b * b;
            dy -= 2 * a * a;
            d1 += dx - dy + (b * b);
        }
    }

    // 2nd D parameter
    d2 = ((b * b) * ((x + 0.5) * (x + 0.5))) +
         ((a * a) * ((y - 1) * (y - 1))) -
         (a * a * b * b);

    // midpoint to bottom Region2:
    while (y >= 0) {
        drawSymmetricPixels(centerX, centerY, x, y, pixels);
        if (d2 > 0) {
            y--;
            dy -= 2 * a * a;
            d2 += (a * a) - dy;
        } else {
            y--;
            x++;
            dx += 2 * b * b;
            dy -= 2 * a * a;
            d2 += dx - dy + (a * a);
        }
    }
}

void ShapeTool::drawSymmetricPixels(int cx, int cy, int x, int y, std::vector<std::vector<Pixel>>& pixels) {
    const std::vector<std::pair<int, int>> offsets = {
        {x, y}, {-x, y}, {x, -y}, {-x, -y}
    };
    for (const auto& offset : offsets) {
        int targetX = cx + offset.first;
        int targetY = cy + offset.second;
        if (targetX >= 0 && targetX < pixels.size() && targetY >= 0 && targetY < pixels[0].size()) {
            pixels[targetX][targetY].setColor(color);
        }
    }
}

void ShapeTool::drawTriangle(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels) {
    //Midpoint for the based
    int baseMidX = (startX + endX) / 2;
    int height = endY - startY;

    for (int y = 0; y <= height; ++y) {
        int leftX = baseMidX - y;
        int rightX = baseMidX + y;

        // Draw the left edge
        if (leftX >= 0 && leftX < pixels.size() && (startY + y) >= 0 && (startY + y) < pixels[0].size()) {
            pixels[leftX][startY + y].setColor(color);
        }

        // Draw the right edge
        if (rightX >= 0 && rightX < pixels.size() && (startY + y) >= 0 && (startY + y) < pixels[0].size()) {
            pixels[rightX][startY + y].setColor(color);
        }
    }

    // baseline should close the triangle.
    int finalLeftX = baseMidX - height;
    int finalRightX = baseMidX + height;
    for (int x = finalLeftX; x <= finalRightX; ++x) {
        if (x >= 0 && x < pixels.size() && endY >= 0 && endY < pixels[0].size()) {
            pixels[x][endY].setColor(color);
        }
    }
}
