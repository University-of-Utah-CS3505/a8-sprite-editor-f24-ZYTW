#include "SymmetryTool.h"

void SymmetryTool::enableSymmetry(bool enabled) {
    symmetryEnabled = enabled;
}

void SymmetryTool::drawSymmetry(QPainter &painter, const QPoint &point, bool horizontal) {
    if (symmetryEnabled) {
        painter.drawPoint(point);
        QPoint mirrorPoint = horizontal ? QPoint(point.x(), -point.y()) : QPoint(-point.x(), point.y());
        painter.drawPoint(mirrorPoint);
    } else {
        painter.drawPoint(point);
    }
}
