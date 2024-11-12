#ifndef SHAPETOOL_H
#define SHAPETOOL_H

#include "Tool.h"
#include <QString>
#include <vector>

class ShapeTool : public Tool {
    Q_OBJECT  // Required since Tool is QObject-based

public:
    explicit ShapeTool(QObject* parent = nullptr) : Tool(parent) {}
    void setShape(const QString& shape) { shapeType = shape; }
    void drawShape(int x1, int y1, int x2, int y2, std::vector<std::vector<Pixel>>& pixels);
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;

private:
    QString shapeType;
};

#endif // SHAPETOOL_H
