#include "StampGallery.h"

StampGallery::StampGallery(QWidget *parent)
    : QWidget(parent), stampList(new QListWidget) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stampList);

    connect(stampList, &QListWidget::itemClicked, this, &StampGallery::selectStamp);
}

void StampGallery::addStamp(const QImage &image, const QString &stampName) {
    stamps[stampName] = image;
    updateStampList();
}

QImage StampGallery::getSelectedStamp() const {
    QListWidgetItem *selectedItem = stampList->currentItem();
    if (selectedItem) {
        QString stampName = selectedItem->text();
        return stamps.value(stampName);
    }
    return QImage();
}

void StampGallery::saveStamp(const std::vector<std::vector<Pixel>>& stampPixels)  {
    QString stampsPath = QApplication::applicationDirPath() + "/stamps";
    QDir dir(stampsPath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".stamp";
    QFile file(dir.filePath(fileName));

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for saving stamp");
        return;
    }

    QJsonObject stampJson;
    QJsonArray pixelArray;

    for (const auto& row : stampPixels) {
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

    stampJson["pixels"] = pixelArray;

    QJsonDocument saveDoc(stampJson);
    file.write(saveDoc.toJson());
    file.close();

    qDebug() << "Stamp saved to:" << file.fileName();
    //emit stampSaved(file.fileName());
}

void StampGallery::loadStampsFromResource() {
    QDir resourceDir(":/stamp_gallery");

    if (!resourceDir.exists()) {
        qWarning() << "Stamp resource directory does not exist.";
        return;
    }

    QStringList stampFiles = resourceDir.entryList(QStringList() << "*.stamp", QDir::Files);

    for (const QString& fileName : stampFiles) {
        QString fullPath = ":/stamp_gallery/" + fileName;
        QFile file(fullPath);

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject json = doc.object();
            displayStampFromJson(json);
        } else {
            qWarning() << "Failed to open stamp file:" << fullPath;
        }
    }
}
void StampGallery::selectStamp() {
    emit stampSelected(getSelectedStamp());
}

void StampGallery::displayStampFromJson(const QJsonObject& json) {
    if (!json.contains("canvasSize") || !json.contains("pixels")) {
        qWarning() << "Invalid stamp JSON format.";
        return;
    }

    int canvasSize = json["canvasSize"].toInt();
    QJsonArray pixelArray = json["pixels"].toArray();

    QImage stampImage(canvasSize, canvasSize, QImage::Format_ARGB32);
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
            stampImage.setPixelColor(i, j, color);
        }
    }

    // Create a QLabel to hold the stamp preview
    QLabel* stampPreview = new QLabel(this);
    stampPreview->setPixmap(QPixmap::fromImage(stampImage.scaled(100, 100, Qt::KeepAspectRatio)));
    stampPreview->setAlignment(Qt::AlignCenter);
    stampPreview->setStyleSheet("border: 1px solid black; margin: 5px;");

    // Add stamp preview to the gallery
    QVBoxLayout* galleryLayout = static_cast<QVBoxLayout*>(layout());
    if (!galleryLayout) {
        galleryLayout = new QVBoxLayout(this);
        setLayout(galleryLayout);
    }
    galleryLayout->addWidget(stampPreview);

}

void StampGallery::updateStampList() {
    stampList->clear();
    for (auto it = stamps.begin(); it != stamps.end(); ++it) {
        stampList->addItem(it.key());
    }
}
