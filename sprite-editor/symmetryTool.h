/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: ZHENGXI ZHANG
 */
#ifndef SYMMETRYTOOL_H
#define SYMMETRYTOOL_H

#include "Tool.h"
#include <vector>

class SymmetryTool : public Tool {
    Q_OBJECT

public:
    explicit SymmetryTool(QObject* parent = nullptr);
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;

    void setCanvasSize(int size);
    void setColor(QColor color) override;

private:
    int canvasSize = 48;
};

#endif // SYMMETRYTOOL_H
