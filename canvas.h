#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <vector>
#include <QColor>
#include "Pixel.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);

    int getCanvasSize() const;

signals:

public slots:
    void setCanvasSize(int size);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int canvasSize;
    std::vector<std::vector<Pixel>> pixels;
    QColor currentColor;

    void initializePixels();
    void drawPosition(int x, int y);
};

#endif // CANVAS_H
