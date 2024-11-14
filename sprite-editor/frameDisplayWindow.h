/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: YINHAO CHEN
 */
#ifndef FRAMEDISPLAYWINDOW_H
#define FRAMEDISPLAYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class FrameDisplayWindow : public QWidget {
    Q_OBJECT
public:
    explicit FrameDisplayWindow(QWidget *parent = nullptr);
    void setFrameImage(const QImage &image);

private:
    QLabel *frameLabel;
};

#endif // FRAMEDISPLAYWINDOW_H
