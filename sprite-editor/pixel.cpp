#include "pixel.h"

Pixel::Pixel(QColor color) : color(color) {}

void Pixel::setColor(QColor color) {
    this->color = color;
}

QColor Pixel::getColor() const {
    return color;
}
