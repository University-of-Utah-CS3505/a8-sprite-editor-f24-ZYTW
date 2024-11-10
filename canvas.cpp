#include "canvas.h"

Canvas::Canvas(QWidget *parent): QWidget(parent)
{
    canvasSize = 30;
    currentImage = QPixmap(canvasSize,canvasSize).toImage();
    currentImage.fill(QColor("#ffffff"));
    penSize = 3;
    framesPerSecond = 24;
    initializePixels();
    penColor = Qt::black;
    setFixedSize(canvasSize*10, canvasSize*10);
}

void Canvas::initializePixels() {
    pixels = std::vector<std::vector<Pixel>>(canvasSize, std::vector<Pixel>(canvasSize, Pixel(Qt::white)));
}

QImage Canvas::getCanvasImage() const {
    return currentImage;
}

void Canvas::setCanvasSize(int size)
{
    canvasSize = size;
    setFixedSize(canvasSize*10, canvasSize*10);
    initializePixels();
    currentImage = QPixmap(canvasSize,canvasSize).toImage();
    currentImage.fill(QColor("#ffffff"));
    update();
    if (!frames.isEmpty()) {
        frames.removeFirst();
    }
    frames.push_back(currentImage);
}

void Canvas::setFramesPerSecond(int fps)
{
    framesPerSecond = fps;
}

void Canvas::setPenColor(QColor color)
{
    penColor = color;
}

// draw each Pixels
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::black);

    for (int i = 0; i < pixels.size(); ++i) {
        for (int j = 0; j < pixels[i].size(); ++j) {
            painter.setBrush(pixels[i][j].getColor());
            painter.drawRect(i * 10, j * 10, 10, 10);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    drawPosition(event->x() / 10, event->y() / 10);
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    drawPosition(event->x() / 10, event->y() / 10);
    update();
}

void Canvas::drawPosition(int x, int y) {
    if (x >= 0 && x < pixels.size() && y >= 0 && y < pixels[0].size()) {
        pixels[x][y].setColor(penColor);
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
