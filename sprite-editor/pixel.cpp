/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: TERESA PARK
 */
#include "pixel.h"

Pixel::Pixel(QColor color) : color(color) {}

void Pixel::setColor(QColor color) {
    this->color = color;
}

QColor Pixel::getColor() const {
    return color;
}
