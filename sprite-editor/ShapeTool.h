/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: ZHENGXI ZHANG
 */
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
    /**
     * @brief drawSymmetricPixels
     *  Draws symmetric pixels on the canvas based on a center point and an offset.
     * @param cx
     * @param cy
     * @param x
     * @param y
     * @param pixels
     */
    void drawSymmetricPixels(int cx, int cy, int x, int y, std::vector<std::vector<Pixel>>& pixels);
    /**
     * @brief drawEllipse
     *  This method was based on the Midpoint ellipse drawing algorithm in
     *  https://www.geeksforgeeks.org/midpoint-ellipse-drawing-algorithm/
     *  and also wrote based on midptellipse in the C++ implementation in that page.
     * @param startX
     * @param startY
     * @param endX
     * @param endY
     * @param pixels
     */
    void drawEllipse(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels);
    /**
     * @brief drawTriangle
     * Based on the Triangle drawing but we only need to frame:
     * https://www.geeksforgeeks.org/cpp-program-to-print-triangle-pattern/
     * @param startX
     * @param startY
     * @param endX
     * @param endY
     * @param pixels
     */
    void drawTriangle(int startX, int startY, int endX, int endY, std::vector<std::vector<Pixel>>& pixels);
};

#endif // SHAPETOOL_H
