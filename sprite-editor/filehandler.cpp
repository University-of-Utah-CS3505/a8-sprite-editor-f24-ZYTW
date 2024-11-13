#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>

#include "filehandler.h"

void FileHandler::saveFile(const QString& path, int framesPerSecond, int canvasSize, const std::vector<std::vector<Pixel>>& pixels) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file");
        return;
    }

    QJsonObject spriteJson;
    spriteJson["framesPerSecond"] = framesPerSecond;
    spriteJson["canvasSize"] = canvasSize;

    QJsonArray pixelArray;
    for (const auto& row : pixels) {
        QJsonArray rowArray;
        for (const auto& pixel : row) {
            QJsonObject pixelJson;
            QColor color = pixel.getColor();
            pixelJson["red"] = color.red();
            pixelJson["green"] = color.green();
            pixelJson["blue"] = color.blue();
            pixelJson["alpha"] = color.alpha();
            rowArray.append(pixelJson);
        }
        pixelArray.append(rowArray);
    }
    spriteJson["pixels"] = pixelArray;

    QJsonDocument saveDoc(spriteJson);
    file.write(saveDoc.toJson());
    file.close();
}

bool FileHandler::loadFile(const QString& path, int& framesPerSecond, int& canvasSize, std::vector<std::vector<Pixel>>& pixels) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for loading");
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(data));
    QJsonObject spriteJson = loadDoc.object();

    if (!spriteJson.contains("framesPerSecond") || !spriteJson.contains("canvasSize") || !spriteJson.contains("pixels")) {
        return false;
    }

    framesPerSecond = spriteJson["framesPerSecond"].toInt();
    canvasSize = spriteJson["canvasSize"].toInt();

    QJsonArray pixelArray = spriteJson["pixels"].toArray();
    pixels.resize(canvasSize, std::vector<Pixel>(canvasSize));

    for (int i = 0; i < pixelArray.size(); ++i) {
        QJsonArray rowArray = pixelArray[i].toArray();
        for (int j = 0; j < rowArray.size(); ++j) {
            QJsonObject pixelJson = rowArray[j].toObject();
            QColor color(
                pixelJson["red"].toInt(),
                pixelJson["green"].toInt(),
                pixelJson["blue"].toInt(),
                pixelJson["alpha"].toInt()
                );
            pixels[i][j].setColor(color);
        }
    }

    file.close();
    return true;
}
