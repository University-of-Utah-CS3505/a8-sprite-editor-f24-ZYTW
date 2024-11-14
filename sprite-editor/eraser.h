/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: WYNTER KIM
 */
#ifndef ERASER_H
#define ERASER_H

#include "Tool.h"
#include "Pixel.h"
#include <QImage>
class Eraser : public Tool {
public:
    Eraser(QColor backgroundColor = Qt::white) { this->color = backgroundColor; }

    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;
};

#endif // ERASER_H
