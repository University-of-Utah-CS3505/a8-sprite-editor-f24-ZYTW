#ifndef ERASER_H
#define ERASER_H

#include "Tool.h"
#include "Pixel.h"

class Eraser : public Tool {
public:
    Eraser(QColor backgroundColor = Qt::white) { this->color = backgroundColor; }

    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;
};

#endif // ERASER_H
