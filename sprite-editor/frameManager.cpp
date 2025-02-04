/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: YINHAO CHEN, ZHENGXI ZHANG
 */
#include "frameManager.h"

FrameManager::FrameManager(QObject *parent) : QObject(parent), playbackTimer(new QTimer(this)) {
    connect(playbackTimer, &QTimer::timeout, this, &FrameManager::startPreview);
    QImage defaultFrame(100, 100, QImage::Format_ARGB32);
}


void FrameManager::addFrame(const QImage &frameImage) {
    frames.append(frameImage);  // Add the canvas image as a new frame
}

void FrameManager::removeFrame(int index) {
    if (index >= 0 && index < frames.size()) {
        QImage deletedFrame = frames.at(index).copy();
        deletedFrames.append(qMakePair(index + 1, deletedFrame));  // Store 1-based index

        frames.removeAt(index);  // Remove from frames list

        // Adjust currentFrameIndex if necessary
        if (currentFrameIndex >= frames.size()) {
            currentFrameIndex = frames.size() - 1;
        }



        emit frameChanged(frames.isEmpty() ? QImage() : frames[qMax(0, currentFrameIndex)]);
    } else {
        qDebug() << "Invalid frame index" << index << "for deletion.";
    }
}

void FrameManager::restoreFrame(int originalIndex, const QImage &frame) {
    int zeroBasedIndex = originalIndex - 1;  // Convert to 0-based index

    // Insert at the closest position within bounds if the index is now invalid
    int insertPosition = (zeroBasedIndex >= frames.size()) ? frames.size() : zeroBasedIndex;
    frames.insert(insertPosition, frame);

    // Remove the restored frame from deletedFrames list
    auto it = std::find_if(deletedFrames.begin(), deletedFrames.end(),
                           [&](const QPair<int, QImage> &pair) {
                               return pair.first == originalIndex;
                           });
    if (it != deletedFrames.end()) {
            deletedFrames.erase(it);
    }

    emit frameChanged(frame);  // Update display
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
QList<QPair<int, QImage>> FrameManager::getDeletedFrames() const {
    return deletedFrames;
}
