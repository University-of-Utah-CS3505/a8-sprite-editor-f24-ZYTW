#ifndef SYMMETRYTOOL_H
#define SYMMETRYTOOL_H
#include <QImage>
#include <QPainter>
#include <QObject>

class SymmetryTool : public QObject {
    Q_OBJECT
public:
    void enableSymmetry(bool enabled);
    void drawSymmetry(QPainter &painter, const QPoint &point, bool horizontal = true);

private:
    bool symmetryEnabled = false;
};
#endif // SYMMETRYTOOL_H
