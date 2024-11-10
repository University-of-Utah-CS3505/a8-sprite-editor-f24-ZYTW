#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <vector>
#include "Pixel.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<std::vector<Pixel>> pixels;
    QColor currentColor;
    void drawPosition(int x, int y);

};

#endif // CANVAS_H
