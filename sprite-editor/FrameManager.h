#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H
#include <QList>
#include <QTimer>

class FrameManager : public QObject {
    Q_OBJECT
public:
    explicit FrameManager(QObject *parent = nullptr);
    void addFrame();
    void removeFrame(int index);
    void setFPS(int fps);
    void startPreview();
    void stopPreview();
    QList<QImage> getFrames() const;

signals:
    void frameChanged(const QImage &frame);

private:
    QList<QImage> frames;
    QTimer *playbackTimer;
    int currentFrameIndex = 0;
};
#endif // FRAMEMANAGER_H
