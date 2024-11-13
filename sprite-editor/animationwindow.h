#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QImage>

class AnimationWindow : public QWidget {
    Q_OBJECT
public:
    explicit AnimationWindow(QWidget *parent = nullptr);

public slots:
    void displayFrame(const QImage &frame);  // Slot to display each frame

private:
    QLabel *frameLabel;  // QLabel to show the frame
};

#endif // ANIMATIONWINDOW_H
