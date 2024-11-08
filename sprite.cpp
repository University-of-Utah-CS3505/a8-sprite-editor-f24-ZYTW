#include "sprite.h"

Sprite::Sprite(QObject *parent): QObject(parent)
{
    canvasSize = 32;
    canvasImage = QPixmap(canvasSize,canvasSize).toImage();
    canvasImage.fill(QColor("#ffffff"));
}

void Sprite::setCanvasSize(int size)
{
    canvasSize = size;
    canvasImage = QPixmap(canvasSize,canvasSize).toImage();
    canvasImage.fill(QColor("#ffffff"));
}

QImage Sprite::getCanvasImage() const
{
    return canvasImage;
}
