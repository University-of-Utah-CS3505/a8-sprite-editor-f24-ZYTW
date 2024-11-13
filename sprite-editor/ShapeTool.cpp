#include "ShapeTool.h"
#include <QDebug>
#include <cmath>

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
    int a = (endX - startX) / 2;
    int b = (endY - startY) / 2;
    int centerX = startX + a;
    int centerY = startY + b;

    int x = 0;
    int y = b;
    int a2 = a * a;
    int b2 = b * b;
    int d1 = b2 - a2 * b + 0.25 * a2;

    while (b2 * x <= a2 * y) {
        drawSymmetricPixels(centerX, centerY, x, y, pixels);
        if (d1 < 0) {
            d1 += b2 * (2 * x + 3);
        } else {
            d1 += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
            y--;
        }
        x++;
    }

    int d2 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;

    while (y >= 0) {
        drawSymmetricPixels(centerX, centerY, x, y, pixels);
        if (d2 > 0) {
            d2 += a2 * (-2 * y + 3);
        } else {
            d2 += b2 * (2 * x + 2) + a2 * (-2 * y + 3);
            x++;
        }
        y--;
    }
}

void ShapeTool::drawSymmetricPixels(int cx, int cy, int x, int y, std::vector<std::vector<Pixel>>& pixels) {
    if (cx + x >= 0 && cx + x < pixels.size() && cy + y >= 0 && cy + y < pixels[0].size())
        pixels[cx + x][cy + y].setColor(color);
    if (cx - x >= 0 && cx - x < pixels.size() && cy + y >= 0 && cy + y < pixels[0].size())
        pixels[cx - x][cy + y].setColor(color);
    if (cx + x >= 0 && cx + x < pixels.size() && cy - y >= 0 && cy - y < pixels[0].size())
        pixels[cx + x][cy - y].setColor(color);
    if (cx - x >= 0 && cx - x < pixels.size() && cy - y >= 0 && cy - y < pixels[0].size())
        pixels[cx - x][cy - y].setColor(color);
}

void ShapeTool::drawTriangle(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels) {
    int baseMidX = (startX + endX) / 2;

    for (int x = startX; x <= endX; ++x) {
        pixels[x][endY].setColor(color);
    }

    float slopeLeft = static_cast<float>(endY - startY) / (baseMidX - startX);
    float slopeRight = static_cast<float>(endY - startY) / (endX - baseMidX);

    for (int y = startY; y <= endY; ++y) {
        int leftX = baseMidX - static_cast<int>((y - startY) / slopeLeft);
        int rightX = baseMidX + static_cast<int>((y - startY) / slopeRight);
        if (leftX >= 0 && leftX < pixels.size())
            pixels[leftX][y].setColor(color);
        if (rightX >= 0 && rightX < pixels.size())
            pixels[rightX][y].setColor(color);
    }
}
