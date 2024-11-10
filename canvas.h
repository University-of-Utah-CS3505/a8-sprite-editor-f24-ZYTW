#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QColorDialog>
#include <QWidget>
#include <QVector>
#include <vector>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QImage>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QDebug>

#include "Pixel.h"


class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);


    QVector<QImage> getFrames();
    QImage getCanvasImage() const;

    void eraseWithPen(const QPoint& start, const QPoint& end, int width);
    void drawWithPen(const QPoint& start, const QPoint& end, const QColor& color, int width);

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
    int canvasSize;
    int penSize;
    int framesPerSecond;
    std::vector<std::vector<Pixel>> pixels;

    void initializePixels();
    void drawPosition(int x, int y);
    QVector<QImage> frames;
    QColor penColor;
    QImage currentImage;
    QPoint currentPoint;
};

#endif // CANVAS_H
