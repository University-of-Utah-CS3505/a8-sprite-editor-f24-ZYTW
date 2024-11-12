#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>

Canvas::Canvas(QWidget *parent): QWidget(parent), currentTool(nullptr)
{
    canvasSize = 30;
    penSize = 3;
    framesPerSecond = 24;
    penColor = Qt::black;
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

    if (currentTool == shapeTool) {
        startPoint = QPoint(x, y);
    } else if (currentTool) {
        currentTool->useTool(x, y, pixels);
    }
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        int x = event->x() / 10;
        int y = event->y() / 10;
        if (currentTool) {
            currentTool->useTool(x, y, pixels);
        }
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (currentTool == shapeTool) {
        int x = event->x() / 10;
        int y = event->y() / 10;

        shapeTool->drawShape(startPoint.x(), startPoint.y(), x, y, pixels);
        startPoint = QPoint();  // Reset startPoint after drawing
        update();
    }
}


void Canvas::saveFile()
{
    QString savePath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::homePath(), "Sprite Files (*.ssp)");
    if (savePath.isEmpty()) return;

    // Save information on JSON object
    QJsonObject spriteJson;
    spriteJson["framesPerSecond"] = framesPerSecond;
    spriteJson["canvasSize"] = canvasSize;

    // Save information on JsonArray by iterating all frames
    QJsonArray framesJsonArray;
    for (const QImage &frameImage: frames){
        QJsonArray framePixels;
        for (int y = 0; y < canvasSize; y++){
            for (int x = 0; x < canvasSize; x++){
                QColor pixelColor = frameImage.pixelColor(x, y);

                QJsonObject pixelJson;
                pixelJson["red"] = pixelColor.red();
                pixelJson["blue"] = pixelColor.blue();
                pixelJson["green"] = pixelColor.green();
                pixelJson["alpha"] = pixelColor.alpha();

                framePixels.append(pixelJson);
            }
        }
        QJsonObject frameJson;
        frameJson["pixels"] = framePixels;
        framesJsonArray.append(frameJson);
    }

    spriteJson["frames"] = framesJsonArray; // Add Json frame data to sprite

    // Open the file to save the JSON data in the file and close
    QJsonDocument spriteDoc(spriteJson);
    QFile saveFile(savePath);
    if (saveFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&saveFile);
        out << spriteDoc.toJson(QJsonDocument::Indented);
        saveFile.close();
    }
    else qWarning("Failed to open file for saving");
}

void Canvas::openFile()
{
    QString openPath = QFileDialog::getOpenFileName(nullptr, "Open File", QDir::homePath(), "Sprite Files (*.ssp)");
    QFile spriteFile(openPath);
    if(!spriteFile.open(QIODevice::ReadOnly | QIODevice::Text)) qWarning("Failed to open file");

    QByteArray jsonData = spriteFile.readAll();
    spriteFile.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if(jsonDoc.isNull() ||!jsonDoc.isObject()) return;
    QJsonObject spriteData = jsonDoc.object();

    if( !spriteData.contains("framesPerSecond") ||
        !spriteData.contains("canvasSize") ||
        !spriteData.contains("frames")) return;

    framesPerSecond = spriteData["framesPerSecond"].toString().toInt();
    canvasSize = spriteData["canvasSize"].toString().toInt();

    QJsonArray framesArray = spriteData["frames"].toArray();
    frames.clear(); // delete original frames

    for (const QJsonValue& frameValue : framesArray) {
        QJsonObject frameData = frameValue.toObject();
        QJsonArray pixelArray = frameData["pixels"].toArray();
        QImage frameImage(canvasSize, canvasSize, QImage::Format_ARGB32);

        int pixelIndex = 0;
        for (int x = 0; x < canvasSize; ++x) {
            for (int y = 0; y < canvasSize; ++y) {
                QJsonObject pixelData = pixelArray[pixelIndex++].toObject();
                QColor pixelColor(
                    pixelData["red"].toInt(),
                    pixelData["green"].toInt(),
                    pixelData["blue"].toInt(),
                    pixelData["alpha"].toInt()
                    );
                frameImage.setPixelColor(x, y, pixelColor);
            }
        }
        frames.append(frameImage); // add new frames
    }
    // TODO:: Update UI
}
