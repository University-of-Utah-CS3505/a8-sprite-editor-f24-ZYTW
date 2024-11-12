#include "ShapeTool.h"
#include <QDebug>

void ShapeTool::drawShape(int x1, int y1, int x2, int y2, std::vector<std::vector<Pixel>> &pixels) {
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 >= pixels.size() || x2 >= pixels.size() || y1 >= pixels[0].size() || y2 >= pixels[0].size()) {
        qWarning() << "ShapeTool: Out-of-bounds pixel coordinates!";
        return;
    }

    int startX = std::min(x1, x2);
    int startY = std::min(y1, y2);
    int endX = std::max(x1, x2);
    int endY = std::max(y1, y2);

    if (shapeType == "Rectangle") {
        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                pixels[x][y].setColor(Qt::black);
            }
        }
    } else if (shapeType == "Ellipse") {
        int a = (endX - startX) / 2;  // Horizontal radius
        int b = (endY - startY) / 2;  // Vertical radius
        int centerX = startX + a;
        int centerY = startY + b;

        for (int x = -a; x <= a; ++x) {
            for (int y = -b; y <= b; ++y) {
                if ((x * x * b * b) + (y * y * a * a) <= (a * a * b * b)) {
                    int px = centerX + x;
                    int py = centerY + y;
                    if (px >= 0 && px < pixels.size() && py >= 0 && py < pixels[0].size()) {
                        pixels[px][py].setColor(Qt::black);
                    }
                }
            }
        }
    } else if (shapeType == "Triangle") {
        QPoint p1(startX, endY);   // Bottom-left
        QPoint p2(endX, endY);     // Bottom-right
        QPoint p3((startX + endX) / 2, startY);  // Top-center

        auto isInsideTriangle = [](QPoint p, QPoint a, QPoint b, QPoint c) -> bool {
            int w1 = (p.x() * (a.y() - c.y()) + (a.x() - c.x()) * p.y() + (c.x() * a.y() - a.x() * c.y()));
            int w2 = (p.x() * (b.y() - a.y()) + (b.x() - a.x()) * p.y() + (a.x() * b.y() - b.x() * a.y()));
            int w3 = (p.x() * (c.y() - b.y()) + (c.x() - b.x()) * p.y() + (b.x() * c.y() - c.x() * b.y()));
            return (w1 <= 0 && w2 <= 0 && w3 <= 0) || (w1 >= 0 && w2 >= 0 && w3 >= 0);
        };

        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                if (isInsideTriangle(QPoint(x, y), p1, p2, p3)) {
                    pixels[x][y].setColor(Qt::black);
                }
            }
        }
    }
}


void ShapeTool::useTool(int x, int y, std::vector<std::vector<Pixel>> &pixels) {
    int x2 = x + 1;  // Default small area for testing
    int y2 = y + 1;  // You can update with variables for wider area based on state

    drawShape(x, y, x2, y2, pixels);
    qDebug() << "Shape tool used at:" << x << y;
}
