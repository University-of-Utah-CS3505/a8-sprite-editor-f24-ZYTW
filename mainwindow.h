#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <canvas.h>

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

public slots:
    // Connecting spot for all connections between slots and signals
    void setUpConnections();

    // Popup on first screen to set canvasSize
    void canvasSizeDialog();
    void updateCanvasDisplay(QPixmap pixmap);

    // Change FPS Speed based on the users' selection
    void updateFPS();

    // display Color Dialog to change the penColor user has selected
    void showColorDialog();

signals:
    void setPenColor(QColor color);

private:
    Ui::MainWindow *ui;
    Canvas *canvas;
    QPoint lastPoint;
    QColorDialog colorDialog;
};
#endif // MAINWINDOW_H
