#include "FrameManager.h"
#include <QImage>
#include <QDebug>

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

        // Renumber deletedFrames and frames to prevent index mismatches
        for (int i = 0; i < frames.size(); ++i) {
            frames[i] = frames[i].copy();  // Reassign to ensure correct ordering
        }

        emit frameChanged(frames.isEmpty() ? QImage() : frames[qMax(0, currentFrameIndex)]);
    } else {
        qDebug() << "Invalid frame index" << index << "for deletion.";
    }
}

void FrameManager::restoreFrame(int originalIndex, const QImage &frame) {
    int zeroBasedIndex = originalIndex - 1;  // Convert to 0-based index

    if (zeroBasedIndex >= 0 && zeroBasedIndex <= frames.size()) {
        frames.insert(zeroBasedIndex, frame.copy());  // Insert an exact copy at the correct position

        // Remove the restored frame from deletedFrames
        auto it = std::find_if(deletedFrames.begin(), deletedFrames.end(),
                               [&](const QPair<int, QImage> &pair) {
                                   return pair.first == originalIndex;
                               });
        if (it != deletedFrames.end()) {
            deletedFrames.erase(it);
        }

        emit frameChanged(frame);  // Update display
    } else {
        qDebug() << "Invalid original index" << originalIndex << "for restoration.";
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
QList<QPair<int, QImage>> FrameManager::getDeletedFrames() const {
    return deletedFrames;
}
