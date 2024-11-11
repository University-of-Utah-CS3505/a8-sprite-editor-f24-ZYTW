#include "colorpalette.h"

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
