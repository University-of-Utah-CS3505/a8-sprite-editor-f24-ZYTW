/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: TERESA PARK
 */
#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QColor>
#include <QWidget>
#include <QColorDialog>
#include <QImage>
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
