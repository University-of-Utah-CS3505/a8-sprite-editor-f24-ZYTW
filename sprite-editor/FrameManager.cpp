#include "FrameManager.h"
#include <QImage>

FrameManager::FrameManager(QObject *parent) : QObject(parent), playbackTimer(new QTimer(this)) {
    connect(playbackTimer, &QTimer::timeout, this, &FrameManager::startPreview);
}

void FrameManager::addFrame() {
    frames.append(QImage(100, 100, QImage::Format_ARGB32));  // Replace with actual canvas size
    frames.last().fill(Qt::transparent);
}

void FrameManager::removeFrame(int index) {
    if (index >= 0 && index < frames.size()) {
        frames.removeAt(index);
    }
}

void FrameManager::setFPS(int fps) {
    playbackTimer->setInterval(1000 / fps);
}

void FrameManager::startPreview() {
    if (frames.isEmpty()) return;
    currentFrameIndex = (currentFrameIndex + 1) % frames.size();
    emit frameChanged(frames[currentFrameIndex]);
}

void FrameManager::stopPreview() {
    playbackTimer->stop();
}

QList<QImage> FrameManager::getFrames() const {
    return frames;
}
