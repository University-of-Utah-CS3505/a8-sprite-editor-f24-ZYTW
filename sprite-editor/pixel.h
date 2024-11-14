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
    /**
     * @brief Pixel
     * Constructor for a Pixel object with specific color, default: white.
     * @param sets the default color to white.
     */
    Pixel(QColor color = QColor(Qt::white));
    /**
     * @brief setColor
     * Sets the color of the pixel.
     * @param color The new color to be assigned to the pixel.
    **/
    void setColor(QColor color);
    /**
     * @brief getColor
     * Gets the color of the pixel.
    **/
    QColor getColor() const;

private:
    QColor color;
};

#endif // PIXEL_H
