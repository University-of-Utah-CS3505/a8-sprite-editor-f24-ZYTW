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
    QList<QImage> getFrames() const;

signals:
    void frameChanged(const QImage &frame);


private:
    QList<QImage> frames;
    QTimer *playbackTimer;
    int currentFrameIndex = 0;
};
#endif // FRAMEMANAGER_H
