#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <canvas.h>
#include "Pen.h"
#include "Eraser.h"
#include "Colorpalette.h"
#include <ShapeTool.h>
#include <FrameManager.h>
#include <StampGallery.h>
#include <SymmetryTool.h>
#include <QImage>
#include <QFileDialog>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

     QImage getCanvasImage() const;

private slots:
    void selectPenTool();
    void selectEraserTool();
    void selectPaletteTool();
    void setPenColor(QColor color);
    void selectShapeTool();
    void setShapeColor(QColor color);
    void setSymmetryColor(QColor color);
    void updateToolButtonHighlight(QPushButton* selectedButton);
    void onSelectStampButtonClicked();
    void applyStampToCanvas(const QJsonObject& stampJson);

public slots:
    // Connecting spot for all connections between slots and signals
    void setUpConnections();

    // Popup on first screen to set canvasSize
    void canvasSizeDialog();
    void updateCanvasDisplay(QPixmap pixmap);

    // Change FPS Speed based on the users' selection
    void updateFPS();

private:
    Ui::MainWindow *ui;
    Canvas *canvas;
    QPoint lastPoint;
    QColorDialog colorDialog;

    Tool* currentTool;
    Pen* penTool;
    Eraser* eraserTool;
    ColorPalette* colorPalette;

    //
    SymmetryTool *symmetryTool;
    StampGallery *stampGallery;
    FrameManager *frameManager;
    ShapeTool* shapeTool;
    int selectedIndex = 0;

    void updateCanvasTool();
};
#endif // MAINWINDOW_H
