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
#include <QInputDialog>
#include <QLineEdit>

class StampGallery : public QWidget {
    Q_OBJECT

public:
    explicit StampGallery(QWidget *parent = nullptr);
    void addStamp(const QImage &image, const QString &stampName);
    QImage getSelectedStamp() const;

signals:
    void stampSelected(const QImage &stamp);

public slots:
    /**
     * @brief It will save the Stamp selected
     * @param stampPixels
     */
    void saveStamp(const std::vector<std::vector<Pixel>>& stampPixels);
    /**
     * @brief Load the Stamp from file
     */
    void loadStamps();

private slots:
    void selectStamp();

private:
    QListWidget *stampList;
    QMap<QString, QImage> stamps;
    /**
     * @brief Display the json stamp file
     * @param json
     */
    void displayStampFromJson(const QJsonObject& json);
    /**
     * @brief it will update the list in StampGallery window
     */
    void updateStampList();
};

#endif // STAMPGALLERY_H
