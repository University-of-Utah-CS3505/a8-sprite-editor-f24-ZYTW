/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: WYNTER KIM, TERESA PARK
 */
#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QColor>
#include <vector>
#include "Pixel.h"

class Tool : public QObject {
    Q_OBJECT

public:
    explicit Tool(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~Tool() = default;
    /**
     * @brief Can let the user use tool at the mouseLocation
     * @param x postion
     * @param y
     * @param pixels
     */
    virtual void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) = 0;
    /**
     * @brief SetColor for the tool
     * @param color
     */
    virtual void setColor(QColor color) { this->color = color; }
    QColor getColor() const { return color; }

protected:
    QColor color;
};

#endif // TOOL_H
