/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: YINHAO CHEN
 */
#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>

class AnimationWindow : public QWidget {
    Q_OBJECT
public:
    explicit AnimationWindow(QWidget *parent = nullptr);

public slots:
    /**
     * @brief Slot to display each frame
     * @param frame
     */
    void displayFrame(const QImage &frame);

private:
    QLabel *frameLabel;
};

#endif // ANIMATIONWINDOW_H
