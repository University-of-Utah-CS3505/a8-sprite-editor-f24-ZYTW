#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <vector>
#include "Pixel.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(int width, int height, QWidget *parent = nullptr);
    void drawPixel(int x, int y, QColor color);
    void erasePixel(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int width;
    int height;
    std::vector<std::vector<Pixel>> pixels;
    QImage image;
};

#endif // CANVAS_H
