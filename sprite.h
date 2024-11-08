#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>
#include <QPixmap>
#include <QObject>
#include <QColor>

class Sprite: public QObject
{
    Q_OBJECT
private:
    int canvasSize;
    QImage canvasImage;

public:
    explicit Sprite(QObject *parent = nullptr);
    QImage getCanvasImage() const;

signals:

public slots:
    void setCanvasSize(int size);
};

#endif // SPRITE_H
