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
    /**
     * @class ColorPalette
     * @brief A UI widget for selecting and managing colors in the sprite editor.
     */
    ColorPalette(QWidget *parent = nullptr);
    /**
     * @brief Gets the currently selected color in the color pallete
     * @return QColor The currently selected color.
     */
    QColor getCurrentColor() const { return currentColor; }

signals:
    /**
     * @brief Signal emitted when a new color is selceted
     * @param Connected slots when a color has been chosen in the palette
     */
    void colorSelected(QColor color);

public slots:
    /**
     * @brief Opens the color dialog for color selection
     */
    void openColorDialog();
    /**
     * @brief Sets the currently selected color
     * @param Color New color of set as a current color
     */
    void setColor(QColor color);

private:
    QColorDialog colorDialog;
    QColor currentColor;
};

#endif // COLORPALETTE_H
