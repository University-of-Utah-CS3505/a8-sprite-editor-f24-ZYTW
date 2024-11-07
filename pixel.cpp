#include "pixel.h"

Pixel::Pixel(QColor color) : color(color) {}

void Pixel::setColor(QColor newColor) {
    color = newColor;
}

QColor Pixel::getColor() const {
    return color;
}
