#ifndef STAMPGALLERY_H
#define STAMPGALLERY_H

#include "pixel.h"
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QImage>
#include <QMap>
#include <QApplication>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>
#include <QListWidgetItem>
#include <QLabel>
#include <QPixmap>

class StampGallery : public QWidget {
    Q_OBJECT

public:
    explicit StampGallery(QWidget *parent = nullptr);
    void addStamp(const QImage &image, const QString &stampName);
    QImage getSelectedStamp() const;

signals:
    void stampSelected(const QImage &stamp);

public slots:
    void saveStamp(const std::vector<std::vector<Pixel>>& stampPixels);
    void loadStampsFromResource();

private slots:
    void selectStamp();

private:
    QListWidget *stampList;
    QMap<QString, QImage> stamps;
    void displayStampFromJson(const QJsonObject& json);

    void updateStampList();
};

#endif // STAMPGALLERY_H
