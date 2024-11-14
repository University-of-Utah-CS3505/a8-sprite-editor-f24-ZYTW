#include "Canvas.h"
#include "qjsonobject.h"

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
    currentStamp = QImage();
    selectingStamp = false;
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

// StampGallery part //
void Canvas::startSelectingStamp() {
    selectingStamp = true;
    currentTool = nullptr;
    emit toolCleared();
}

void Canvas::stopSelectingStamp() {
    if (!selectingStamp) {
        qDebug() << "stopSelectingStamp called but selectingStamp is false.";
        return;
    }
    selectingStamp = false;
    QImage selectedStamp = getCurrentSprite().copy(selectedArea.normalized());
    emit stampSelected(selectedStamp);
    update();
}

QImage Canvas::getCurrentSprite() {
    QImage image(size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent); // Fill with transparency
    QPainter painter(&image);
    this->render(&painter); // Renders the current canvas onto the QImage
    return image;
}

std::vector<std::vector<Pixel>> Canvas::getSelectedPixels() const {
    std::vector<std::vector<Pixel>> selectedPixels;
    int startX = selectedArea.left() / 10;
    int startY = selectedArea.top() / 10;
    int width = selectedArea.width() / 10;
    int height = selectedArea.height() / 10;

    for (int x = startX; x < startX + width; ++x) {
        std::vector<Pixel> row;
        for (int y = startY; y < startY + height; ++y) {
            row.push_back(pixels[x][y]);
        }
        selectedPixels.push_back(row);
    }

    return selectedPixels;
}

// Stamp Gallery part
void Canvas::setCurrentStamp(const QJsonObject& stampJson) {
    currentStampJson = stampJson;
}

void Canvas::applyStamp(const QJsonObject& stampJson, int offsetX, int offsetY) {
    if (!stampJson.contains("pixels")) {
        qWarning("Stamp JSON does not contain pixel data");
        return;
    }

    QJsonArray pixelArray = stampJson["pixels"].toArray();

    for (int i = 0; i < pixelArray.size(); ++i) {
        QJsonArray rowArray = pixelArray[i].toArray();
        for (int j = 0; j < rowArray.size(); ++j) {
            int targetX = i + offsetX;
            int targetY = j + offsetY;

            if (targetX >= 0 && targetX < pixels.size() && targetY >= 0 && targetY < pixels[0].size()) {
                QJsonObject pixelJson = rowArray[j].toObject();
                QColor color(
                    pixelJson["red"].toInt(),
                    pixelJson["green"].toInt(),
                    pixelJson["blue"].toInt(),
                    pixelJson["alpha"].toInt()
                    );
                pixels[targetX][targetY].setColor(color);
            }
        }
    }

    update();  // Repaint canvas
}

void Canvas::setStamp(const QImage &stamp) {
    currentStamp = stamp;
    currentTool = nullptr;
    emit toolCleared();
}
//*********************************************************************//

// FileHandlerPart
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
//*********************************************************************//

// MouseEvent and Paint
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            painter.setBrush(pixels[i][j].getColor());
            painter.drawRect(i * 10, j * 10, 10, 10);
        }
    }
    // StampGallery: selecting
    if (selectingStamp) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(QRect(selectionStart * 10, selectionEnd * 10).normalized());
    }
    if (!currentStamp.isNull()) {
        painter.setOpacity(1.0);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    int x = event->x() / 10;
    int y = event->y() / 10;
    //Handle put stamp and selecting Stamp
    if (!currentStamp.isNull()) {
        for (int i = 0; i < currentStamp.width(); ++i) {
            for (int j = 0; j < currentStamp.height(); ++j) {
                int targetX = x + i;
                int targetY = y + j;

                if (targetX >= 0 && targetX < canvasSize && targetY >= 0 && targetY < canvasSize) {
                    QColor color = currentStamp.pixelColor(i, j);
                    if (color.alpha() > 0) {  // Only apply non-transparent pixels
                        pixels[targetX][targetY].setColor(color);
                    }
                }
            }
        }
        update();
        return;
    }
    if (selectingStamp){
        selectionStart = QPoint(x, y);
        selectionEnd = selectionStart;
    }
    //Handle pentool, symmetric and Shape and eraser
    if (currentTool && currentStamp.isNull()) {
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
    //Handle selecting Stamp
    if (selectingStamp && (event->buttons() & Qt::LeftButton)) {
        selectionEnd = event->pos() / 10;
        update();
    } // Tool
    else if (currentTool && !(dynamic_cast<ShapeTool*>(currentTool))) {
        int x = event->x() / 10;
        int y = event->y() / 10;
        currentTool->useTool(x, y, pixels);
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    int x = event->x() / 10;
    int y = event->y() / 10;
    //Handle selecting Stamp
    if (selectingStamp) {
        selectionEnd = QPoint(x, y);
        selectedArea = QRect(selectionStart, selectionEnd).normalized();
        selectedArea.setTopLeft(selectedArea.topLeft() * 10);
        selectedArea.setBottomRight(selectedArea.bottomRight() * 10);
        stopSelectingStamp();
    } // Tool
    else if (currentTool) {
        currentTool->useTool(x, y, pixels);
    }
    update();
}

