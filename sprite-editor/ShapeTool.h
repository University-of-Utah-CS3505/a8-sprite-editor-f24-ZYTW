#ifndef SHAPETOOL_H
#define SHAPETOOL_H
#include <QPainter>
#include <QMouseEvent>

class ShapeTool : public QObject {
    Q_OBJECT
public:
    explicit ShapeTool(QObject *parent = nullptr);
    void setShape(const QString &shape);
    void drawShape(QPainter &painter, const QPoint &start, const QPoint &end);

private:
    QString shapeType;
};
#endif // SHAPETOOL_H
