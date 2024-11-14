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

    // We want to get a single name from user, just using QInputDialog.
    bool ok;
    QString userInputName = QInputDialog::getText(
        this,
        tr("Save Stamp"),
        tr("Enter a name for the stamp:"),
        QLineEdit::Normal,
        QString(),
        &ok
        );
    // user cancel, just return.
    if (!ok) {
        qDebug() << "User canceled stamp saving.";
        return;
    }

    // Use default name if input is empty, "Stamp_hhmmss"
    QString stampName = userInputName.trimmed().isEmpty()
                            ? "stamp_" + QDateTime::currentDateTime().toString("hhmmss")
                            : userInputName.trimmed();

    QString fileName = stampName + ".stamp";
    QFile file(dir.filePath(fileName));

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for saving stamp");
        return;
    }

    QJsonObject stampJson;
    QJsonArray pixelArray;
    // Using the same strategy as save files.
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
    stampJson["name"] = stampName;

    QJsonDocument saveDoc(stampJson);
    file.write(saveDoc.toJson());
    file.close();
}

void StampGallery::loadStamps() {
    QString stampsPath = QApplication::applicationDirPath() + "/stamps";
    QDir resourceDir(stampsPath);
    qDebug() << "Loading stamps from path:" << stampsPath;
    if (!resourceDir.exists()) {
        qWarning() << "Stamp resource directory does not exist:" << stampsPath;
        return;
    }

    QStringList stampFiles = resourceDir.entryList(QStringList() << "*.stamp", QDir::Files);

    for (const QString& fileName : stampFiles) {
        QString fullPath = resourceDir.filePath(fileName);
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

    updateStampList();
}

void StampGallery::selectStamp() {
    QImage selectedStamp = getSelectedStamp();

    if (!selectedStamp.isNull()) {
        emit stampSelected(selectedStamp);
    } else {
        qWarning() << "No stamp selected.";
    }
}

void StampGallery::displayStampFromJson(const QJsonObject& json) {
    if (!json.contains("pixels")) {
        qWarning() << "Invalid stamp JSON format.";
        return;
    }

    QJsonArray pixelArray = json["pixels"].toArray();
    int height = pixelArray.size();
    int width = height > 0 ? pixelArray[0].toArray().size() : 0;

    QImage stampImage(height, width, QImage::Format_ARGB32);
    for (int y = 0; y < height; ++y) {
        QJsonArray rowArray = pixelArray[y].toArray();
        for (int x = 0; x < width; ++x) {
            QJsonObject pixelJson = rowArray[x].toObject();
            QColor color(
                pixelJson["red"].toInt(),
                pixelJson["green"].toInt(),
                pixelJson["blue"].toInt(),
                pixelJson["alpha"].toInt()
                );
            stampImage.setPixelColor(y, x, color); // Correctly map x, y
        }
    }

    QString stampName = json["name"].toString();
    stamps.insert(stampName, stampImage);
}

void StampGallery::updateStampList() {
    stampList->clear();
    for (auto it = stamps.begin(); it != stamps.end(); ++it) {
        stampList->addItem(it.key());
    }
}
