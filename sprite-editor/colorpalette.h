#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QColor>
#include <QWidget>
#include <QColorDialog>

class ColorPalette : public QWidget
{
    Q_OBJECT
public:
    ColorPalette(QWidget *parent = nullptr);

    QColor getCurrentColor() const { return currentColor; }

signals:
    void colorSelected(QColor color);

public slots:
    void openColorDialog();
    void setColor(QColor color);

private:
    QColorDialog colorDialog;
    QColor currentColor;
};

#endif // COLORPALETTE_H
