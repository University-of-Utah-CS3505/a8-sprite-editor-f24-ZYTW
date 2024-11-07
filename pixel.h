#ifndef PIXEL_H
#define PIXEL_H

#include <QColor>

class Pixel
{
public:
    Pixel(QColor color = QColor(Qt::white));
    void setColor(QColor newColor);
    QColor getColor() const;

private:
    QColor color;
};

#endif // PIXEL_H
