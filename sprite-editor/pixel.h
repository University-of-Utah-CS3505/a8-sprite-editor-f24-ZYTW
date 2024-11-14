/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: TERESA PARK
 */
#ifndef PIXEL_H
#define PIXEL_H

#include <QColor>
#include <QImage>

class Pixel
{
public:
    Pixel(QColor color = QColor(Qt::white));
    void setColor(QColor color);
    QColor getColor() const;

private:
    QColor color;
};

#endif // PIXEL_H
