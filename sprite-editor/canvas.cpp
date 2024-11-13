#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>

#include "Canvas.h"

Canvas::Canvas(QWidget *parent): QWidget(parent), currentTool(nullptr)
{
    canvasSize = 30;
    penSize = 3;
    framesPerSecond = 24;
    setFixedSize(canvasSize * 10, canvasSize * 10);

    initializePixels();
}

void Canvas::initializePixels() {
    pixels = std::vector<std::vector<Pixel>>(canvasSize, std::vector<Pixel>(canvasSize, Pixel(Qt::white)));
}

void Canvas::setTool(Tool* tool) {
    currentTool = tool;
}

//Related to StampGallery
QImage Canvas::getCurrentSprite() {
    QImage image(size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent); // Fill with transparency
    QPainter painter(&image);
    this->render(&painter); // Renders the current canvas onto the QImage
    return image;
}

QImage Canvas::getCanvasImage() const {
    QImage image(canvasSize, canvasSize, QImage::Format_ARGB32);

    for (int i = 0; i < canvasSize; ++i) {
        for (int j = 0; j < canvasSize; ++j) {
            image.setPixelColor(i, j, pixels[i][j].getColor());
        }
    }
    return image;
}

void Canvas::setCanvasSize(int size) {
    canvasSize = size;
    setFixedSize(canvasSize * 10, canvasSize * 10);
    initializePixels();
    update();
}

void Canvas::setFramesPerSecond(int fps)
{
    framesPerSecond = fps;
}

void Canvas::setPenColor(QColor color) {
    if (currentTool) {
        currentTool->setColor(color);
    }
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            painter.setBrush(pixels[i][j].getColor());
            painter.drawRect(i * 10, j * 10, 10, 10);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    int x = event->x() / 10;
    int y = event->y() / 10;

    if (currentTool) {
        if (dynamic_cast<ShapeTool*>(currentTool)) {
            auto shapeTool = static_cast<ShapeTool*>(currentTool);
            shapeTool->startDragging(QPoint(x, y));
        } else {
            currentTool->useTool(x, y, pixels);
        }
    }
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (currentTool && !(dynamic_cast<ShapeTool*>(currentTool))) {
        int x = event->x() / 10;
        int y = event->y() / 10;
        currentTool->useTool(x, y, pixels);
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    int x = event->x() / 10;
    int y = event->y() / 10;

    if (currentTool) {
        currentTool->useTool(x, y, pixels);
    }
    update();
}

void Canvas::saveCanvas() {
    QString path = QFileDialog::getSaveFileName(this, "Save Canvas", QDir::homePath(), "Sprite Files (*.ssp)");
    if (!path.isEmpty()) {
        fileHandler.saveFile(path, framesPerSecond, canvasSize, pixels);
    }
}

void Canvas::loadCanvas() {
    QString path = QFileDialog::getOpenFileName(this, "Open Canvas", QDir::homePath(), "Sprite Files (*.ssp)");
    if (!path.isEmpty()) {
        fileHandler.loadFile(path, framesPerSecond, canvasSize, pixels);
        setFixedSize(canvasSize * 10, canvasSize * 10);
        update();
    }
}

