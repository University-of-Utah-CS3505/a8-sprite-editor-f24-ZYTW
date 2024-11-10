#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>

Canvas::Canvas(QWidget *parent) : QWidget(parent), currentColor(Qt::black), canvasSize(32) {
    initializePixels();
    setFixedSize(canvasSize*10, canvasSize*10);
}

void Canvas::initializePixels() {
    pixels = std::vector<std::vector<Pixel>>(canvasSize, std::vector<Pixel>(canvasSize, Pixel(Qt::white)));
}

void Canvas::setCanvasSize(int size) {
    canvasSize = size;
    setFixedSize(canvasSize*10, canvasSize*10);
    initializePixels();
    update();
}

int Canvas::getCanvasSize() const {
    return canvasSize;
}

// draw each Pixels
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::white);

    for (int i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            painter.setBrush(pixels[i][j].getColor());
            painter.drawRect(i * 10, j * 10, 10, 10);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    drawPosition(event->x() / 10, event->y() / 10);
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    drawPosition(event->x() / 10, event->y() / 10);
    update();
}

void Canvas::drawPosition(int x, int y) {
    if (x >= 0 && x < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[x][y].setColor(currentColor);
    }
}
