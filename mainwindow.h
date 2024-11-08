#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "sprite.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(Sprite& sprite, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Sprite &sprite;

signals:
    void canvasSizeChanged(int size);
public slots:
    void clickedCanvasDialog();

    void updateCanvasDisplay();
};
#endif // MAINWINDOW_H
