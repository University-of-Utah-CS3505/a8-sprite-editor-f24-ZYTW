#include "FrameDisplayWindow.h"

FrameDisplayWindow::FrameDisplayWindow(QWidget *parent)
    : QWidget(parent), frameLabel(new QLabel(this)) {
    frameLabel = new QLabel(this);
    frameLabel->setAlignment(Qt::AlignCenter);

    // Layout to manage frameLabel in the window
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(frameLabel);
    setLayout(layout);
}

void FrameDisplayWindow::setFrameImage(const QImage &image) {
    frameLabel->setPixmap(QPixmap::fromImage(image).scaled(frameLabel->size(), Qt::KeepAspectRatio));
}
