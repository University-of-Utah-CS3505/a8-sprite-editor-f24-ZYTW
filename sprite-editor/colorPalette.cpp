/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: TERESA PARK
 */
#include "Colorpalette.h"

ColorPalette::ColorPalette(QWidget *parent) : QWidget(parent), currentColor(Qt::black) {
    connect(&colorDialog, &QColorDialog::colorSelected, this, &ColorPalette::setColor);
}

void ColorPalette::openColorDialog() {
    colorDialog.open();
}

void ColorPalette::setColor(QColor color) {
    if (color.isValid()) {
        currentColor = color;
        emit colorSelected(currentColor);
    }
}
