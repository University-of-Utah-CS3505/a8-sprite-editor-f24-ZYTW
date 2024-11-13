#include "FrameManager.h"
#include <QImage>
#include <QDebug>

FrameManager::FrameManager(QObject *parent) : QObject(parent), playbackTimer(new QTimer(this)) {
    connect(playbackTimer, &QTimer::timeout, this, &FrameManager::startPreview);
    QImage defaultFrame(100, 100, QImage::Format_ARGB32);
}


void FrameManager::addFrame(const QImage &frameImage) {
    frames.append(frameImage);  // Add the canvas image as a new frame
    qDebug() << "Frame added, total frames:" << frames.size();
}

void FrameManager::removeFrame(int index) {
    if (index >= 0 && index < frames.size()) {
        frames.removeAt(index);  // Remove the frame at the specified index
        qDebug() << "Frame at index" << index << "removed. Total frames:" << frames.size();

        // Adjust current frame index if necessary
        if (currentFrameIndex >= frames.size()) {
            currentFrameIndex = frames.size() - 1;  // Set to the last frame if the current index is out of bounds
        }

        // Optionally emit a signal to update the UI or notify other components
        if (!frames.isEmpty()) {
            emit frameChanged(frames[qMax(0, currentFrameIndex)]);
        }
    } else {
        qDebug() << "Invalid index" << index << ". Cannot remove frame.";
    }
}

void FrameManager::setFPS(int fps) {
    if (fps > 0) {
        playbackTimer->setInterval(1000 / fps);  // Adjust the interval based on FPS
    }
}

void FrameManager::startPreview() {
    if (frames.isEmpty()) {
        qDebug() << "No frames available to preview.";
        return;
    }

    if (!playbackTimer->isActive()) {
        playbackTimer->start();
    }

    currentFrameIndex = (currentFrameIndex + 1) % frames.size();
    emit frameChanged(frames[currentFrameIndex]);
}

void FrameManager::stopPreview() {
    playbackTimer->stop();
}

QList<QImage> FrameManager::getFrames() const {
    return frames;
}
