#ifndef SYMMETRYTOOL_H
#define SYMMETRYTOOL_H

#include "Tool.h"
#include <vector>

class SymmetryTool : public Tool {
    Q_OBJECT

public:
    explicit SymmetryTool(QObject* parent = nullptr);
    void useTool(int x, int y, std::vector<std::vector<Pixel>>& pixels) override;

    void setCanvasSize(int size);  // 确保能正确计算对称
    void setColor(QColor color) override;

private:
    int canvasSize = 30;  // 默认画布大小
};

#endif // SYMMETRYTOOL_H
