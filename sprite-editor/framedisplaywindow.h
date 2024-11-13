#ifndef FRAMEDISPLAYWINDOW_H
#define FRAMEDISPLAYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class FrameDisplayWindow : public QWidget {
    Q_OBJECT
public:
    explicit FrameDisplayWindow(QWidget *parent = nullptr);
    void setFrameImage(const QImage &image);

private:
    QLabel *frameLabel;
};

#endif // FRAMEDISPLAYWINDOW_H
