/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: WYNTER KIM, TERESA PARK
 */
#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QColor>
#include <QColorDialog>
#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <vector>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>

#include "Pixel.h"
#include "Tool.h"
#include "FileHandler.h"
#include "shapeTool.h"


class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);


    QVector<QImage> getFrames();
    QImage getCanvasImage() const;
    int getCanvasSize() const;

    void setTool(Tool* tool);

    /**
     * @brief Get the pixels in current sprite
     * @return
     */
    QImage getCurrentSprite();
    void startSelectingStamp();
    void stopSelectingStamp();
    void applyStamp(const QJsonObject& stampJson, int offsetX, int offsetY);
    void setCurrentStamp(const QJsonObject& stampJson);
    /**
     * @brief get the pixle selected when select stamp
     * @return
     */
    std::vector<std::vector<Pixel>> getSelectedPixels() const;


signals:
    void updateCanvasDisplay(QPixmap);
    void stampSelected(const QImage& stamp);
    void toolCleared();

public slots:
    void setCanvasSize(int size);
    void setFramesPerSecond(int fps);
    void setPenColor(QColor color);

    void saveCanvas();
    void loadCanvas();
    void setStamp(const QImage &stamp);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // Comman usage
    void initializePixels();
    Tool* currentTool;
    int canvasSize;
    int framesPerSecond;
    int penSize;
    std::vector<std::vector<Pixel>> pixels;
    // For shapeTool
    bool isDragging = false;
    QPoint startPoint;
    QPoint endPoint;
    // For stampGallery
    bool selectingStamp = false;
    QPoint selectionStart;
    QPoint selectionEnd;
    QRect selectedArea;
    QJsonObject currentStampJson;
    QImage currentStamp;
    QPoint currentStampPosition;
    // For Filehandler
    FileHandler fileHandler;
    // Frames
    QVector<QImage> frames;
};

#endif // CANVAS_H
