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
