#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>

Canvas::Canvas(int width, int height, QWidget *parent)
    : QWidget(parent), width(width), height(height), image(width, height, QImage::Format_RGB32) {
    image.fill(Qt::white);

    pixels.resize(width, std::vector<Pixel>(height, Pixel(Qt::white)));
}

void Canvas::drawPixel(int x, int y, QColor color) {
    if (x < 0 || x > width || y < 0 || y > height) {
        return;
    }

    QPainter painter(&image);
    painter.setPen(color);
    painter.drawPoint(x, y);

    pixels[x][y].setColor(color);
    update(QRect(x, y, 1, 1));
}

void Canvas::erasePixel(int x, int y) {
    drawPixel(x, y, Qt::white);
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawPixel(event->x(), event->y(), Qt::black);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        drawPixel(event->x(), event->y(), Qt::black);
    }
}
