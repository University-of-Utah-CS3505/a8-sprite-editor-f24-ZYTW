#ifndef PIXEL_H
#define PIXEL_H

#include <QColor>
#include <QImage>

class Pixel
{
public:
    Pixel(QColor color = QColor(Qt::white));
    void setColor(QColor color);
    QColor getColor() const;

private:
    QColor color;
};

#endif // PIXEL_H
