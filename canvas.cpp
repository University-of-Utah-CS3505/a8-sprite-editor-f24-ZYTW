#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>

Canvas::Canvas(QWidget *parent) : QWidget(parent), currentColor(Qt::black) {
    int width = 40;
    int height = 40;
    pixels = std::vector<std::vector<Pixel>>(width, std::vector<Pixel>(height, Pixel(Qt::white)));
    setFixedSize(width*10, height*10);
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            painter.setBrush(pixels[i][j].getColor()); // Set color of pixel
            painter.drawRect(i * 10, j * 10, 10, 10);  // Draw 10x10 square
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    drawPosition(event->x() / 10, event->y() / 10); // Calculate which pixel was clicked
    update(); // Repaint to show changes
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    drawPosition(event->x() / 10, event->y() / 10); // Continue drawing as mouse moves
    update(); // Repaint to show changes
}

void Canvas::drawPosition(int x, int y) {
    if (x >= 0 && x < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[x][y].setColor(currentColor); // Set color of specific pixel
    }
}
