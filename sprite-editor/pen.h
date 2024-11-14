/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 */
#ifndef PEN_H
#define PEN_H

#include "Tool.h"
#include "Pixel.h"
#include <QImage>

class Pen : public Tool {
public:
    Pen(QColor color) { this->color = color; }
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;};

#endif // PEN_H
