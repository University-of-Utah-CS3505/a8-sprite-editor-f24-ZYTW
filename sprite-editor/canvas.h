#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QColor>
#include <QColorDialog>
#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <vector>

#include "Pixel.h"
#include "ShapeTool.h"
#include "Tool.h"
#include "FileHandler.h"


class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);


    QVector<QImage> getFrames();
    QImage getCanvasImage() const;
    // Stamp Gallery
    QImage getCurrentSprite() ;

    void setTool(Tool* tool);

signals:
    void updateCanvasDisplay(QPixmap);

public slots:
    void setCanvasSize(int size);
    void setFramesPerSecond(int fps);
    void setPenColor(QColor color);

    void saveCanvas();
    void loadCanvas();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Tool* currentTool;
    int canvasSize;
    int framesPerSecond;
    int penSize;
    std::vector<std::vector<Pixel>> pixels;
    bool isDragging = false;
    QPoint startPoint;
    QPoint endPoint;

    void initializePixels();
    FileHandler fileHandler;
    QVector<QImage> frames;
};

#endif // CANVAS_H
