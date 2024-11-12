#ifndef STAMPGALLERY_H
#define STAMPGALLERY_H
#include <QList>
#include <QImage>
#include <QObject>

class StampGallery : public QObject {
    Q_OBJECT
public:
    void saveStamp(const QImage &stamp);
    QList<QImage> getStamps() const;

private:
    QList<QImage> stamps;
};
#endif // STAMPGALLERY_H
