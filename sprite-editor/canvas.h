#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QColorDialog>
#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <vector>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QDebug>

#include "Pixel.h"
#include "Tool.h"


class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);


    QVector<QImage> getFrames();
    QImage getCanvasImage() const;

    void setTool(Tool* tool);

signals:
    void updateCanvasDisplay(QPixmap);

public slots:
    void setCanvasSize(int size);
    void setFramesPerSecond(int fps);
    void setPenColor(QColor color);
    void openFile();
    void saveFile();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Tool* currentTool;
    int canvasSize;
    int framesPerSecond;
    int penSize;
    std::vector<std::vector<Pixel>> pixels;

    void initializePixels();
    QVector<QImage> frames;
    QColor penColor;
    QImage currentImage;
    QPoint currentPoint;
};

#endif // CANVAS_H
