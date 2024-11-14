/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: YINHAO CHEN
 */
#include "FrameDisplayWindow.h"

FrameDisplayWindow::FrameDisplayWindow(QWidget *parent)
    : QWidget(parent), frameLabel(new QLabel(this)) {

    frameLabel->setAlignment(Qt::AlignCenter);

    // Layout to manage frameLabel in the window
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(frameLabel);
    setLayout(layout);
}

void FrameDisplayWindow::setFrameImage(const QImage &image) {
    frameLabel->setPixmap(QPixmap::fromImage(image).scaled(frameLabel->size(), Qt::KeepAspectRatio));
}
