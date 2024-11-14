/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: YINHAO CHEN, ZHENGXI ZHANG
 */
#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H
#include <QList>
#include <QTimer>
#include <QImage>
#include <QDebug>

class FrameManager : public QObject {
    Q_OBJECT
public:
    explicit FrameManager(QObject *parent = nullptr);
    void addFrame(const QImage &frameImage);
    void removeFrame(int index);
    void setFPS(int fps);
    void stopPreview();
    void startPreview();
    void restoreFrame(int originalIndex, const QImage &frame);
    QList<QImage> getFrames() const;
    QList<QPair<int, QImage>> getDeletedFrames() const;

signals:
    void frameChanged(const QImage &frame);


private:
    QList<QImage> frames;
    QList<QPair<int, QImage>> deletedFrames;
    QTimer *playbackTimer;
    int currentFrameIndex = 1;
};
#endif // FRAMEMANAGER_H
