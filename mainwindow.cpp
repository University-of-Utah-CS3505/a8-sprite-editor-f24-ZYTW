#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Canvas.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Canvas *canvas = new Canvas(800, 600, this);
    setCentralWidget(canvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}
