#include "ShapeTool.h"

void ShapeTool::setShape(const QString &shape) {
    shapeType = shape;
}

void ShapeTool::drawShape(QPainter &painter, const QPoint &start, const QPoint &end) {
    if (shapeType == "Rectangle") {
        painter.drawRect(QRect(start, end));
    } else if (shapeType == "Ellipse") {
        painter.drawEllipse(QRect(start, end));
    }
    // Add more shapes as needed
}
