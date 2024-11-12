#include "StampGallery.h"

void StampGallery::saveStamp(const QImage &stamp) {
    stamps.append(stamp);
}

QList<QImage> StampGallery::getStamps() const {
    return stamps;
}
