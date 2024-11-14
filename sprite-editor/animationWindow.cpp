/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: YINHAO CHEN
 */
#include "AnimationWindow.h"
#include <QPixmap>

AnimationWindow::AnimationWindow(QWidget *parent) : QWidget(parent) {
    // Set up QLabel to display the frames
    frameLabel = new QLabel(this);
    frameLabel->setAlignment(Qt::AlignCenter);

    // Layout to manage frameLabel in the window
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(frameLabel);
    setLayout(layout);
}

void AnimationWindow::displayFrame(const QImage &frame) {
    if (frameLabel) {
        frameLabel->setPixmap(QPixmap::fromImage(frame).scaled(frameLabel->size(), Qt::KeepAspectRatio));
    }
}
