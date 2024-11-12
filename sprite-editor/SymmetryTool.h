#ifndef SYMMETRYTOOL_H
#define SYMMETRYTOOL_H

#include "pixel.h"
#include <QImage>
#include <QPainter>
#include <QObject>
#include <QPoint>

class SymmetryTool : public QObject {
    Q_OBJECT
public:
    explicit SymmetryTool(QObject* parent = nullptr);

    void enableSymmetry(bool enabled);  // Toggle symmetry mode
    bool isSymmetryEnabled() const;     // Check symmetry state

    void drawSymmetry(std::vector<std::vector<Pixel>>& pixels, const QPoint& point, bool horizontal = true);

private:
    bool symmetryEnabled = false;  // Symmetry state
};

#endif // SYMMETRYTOOL_H
