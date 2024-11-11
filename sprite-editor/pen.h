#ifndef PEN_H
#define PEN_H

#include "Tool.h"
#include "Pixel.h"

class Pen : public Tool {
public:
    Pen(QColor color) { this->color = color; }
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;};

#endif // PEN_H
