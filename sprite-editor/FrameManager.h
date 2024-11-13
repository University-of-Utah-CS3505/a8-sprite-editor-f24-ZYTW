#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H
#include <QList>
#include <QTimer>
#include <QImage>

class FrameManager : public QObject {
    Q_OBJECT
public:
    explicit FrameManager(QObject *parent = nullptr);
    void addFrame(const QImage &frameImage);
    void removeFrame(int index);
    void setFPS(int fps);
    void stopPreview();
    void startPreview();
    void restoreFrame(int originalIndex, const QImage &frame);
    QList<QImage> getFrames() const;
    QList<QPair<int, QImage>> getDeletedFrames() const;

signals:
    void frameChanged(const QImage &frame);


private:
    QList<QImage> frames;
    QList<QPair<int, QImage>> deletedFrames;
    QTimer *playbackTimer;
    int currentFrameIndex = 1;
};
#endif // FRAMEMANAGER_H
