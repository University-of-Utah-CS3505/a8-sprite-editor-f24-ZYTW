/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: TERESA PARK
 */
#ifndef PENTOOL_H
#define PENTOOL_H

#include "Tool.h"
#include "Pixel.h"
#include <QImage>

class Pen : public Tool {
public:
    /**
     * @brief Initializaes a Pen object with a specified color to paint pixels on the grid.
     * @param Selected Color of the pen.
     */
    Pen(QColor color) { this->color = color; }
    /**
     * @brief Initializaes a Pen object with a specified color to paint pixels on the grid
     * @param x The x-coordinate of the pixel to be colored.
     * @param y The y-coordinate of the pixel to be colored.
     * @param a vector of the Pixel obects on the pixel grid.
     */
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;};

#endif // PENTOOL_H
