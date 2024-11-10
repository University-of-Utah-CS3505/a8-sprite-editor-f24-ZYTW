#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Canvas.h"

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

private slots:
    void canvasSizeConfirmation();

private:
    Ui::MainWindow *ui;
    Canvas *canvas;

    void setUpConnections();
    void updateCanvasSizeLabel();
};
#endif // MAINWINDOW_H
