#ifndef SHAPETOOL_H
#define SHAPETOOL_H

#include "Tool.h"
#include <QString>
#include <vector>

class ShapeTool : public Tool {
    Q_OBJECT

public:
    explicit ShapeTool(QObject* parent = nullptr, const QString& initialShapeType = "Rectangle");
    void setShapeType(const QString& type);
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;
    void startDragging(const QPoint& start);
    void stopDragging();

private:
    QString shapeType;
    QPoint startPoint;
    bool isDragging;
    QPoint endPoint;
    void drawShape(std::vector<std::vector<Pixel>>& pixels);
    void drawSymmetricPixels(int cx, int cy, int x, int y, std::vector<std::vector<Pixel>>& pixels);
    void drawEllipse(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels);
    void drawTriangle(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels);
};

#endif // SHAPETOOL_H
