#ifndef TOOL_H
#define TOOL_H

#include <QColor>
#include <QPoint>
#include <QImage>
#include "Pixel.h"

class Tool {
public:
    virtual ~Tool() = default;

    virtual void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) = 0;
    virtual void setColor(QColor color) { this->color = color; }
    QColor getColor() const { return color; }

protected:
    QColor color;
};

#endif // TOOL_H
