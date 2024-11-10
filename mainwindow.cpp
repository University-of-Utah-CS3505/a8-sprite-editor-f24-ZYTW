#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this))
{
    ui->setupUi(this);

    if (!ui->canvas->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->canvas);
        ui->canvas->setLayout(layout);
    }

    ui->canvas->layout()->addWidget(canvas);

    setUpConnections();
    updateCanvasDisplay(QPixmap::fromImage(canvas->getCanvasImage().scaled(400,400)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpConnections()
{
    connect(ui->inputButton, &QPushButton::clicked, this, &MainWindow::canvasSizeDialog);
    connect(ui->slider_fps, &QSlider::valueChanged, this, &MainWindow::updateFPS);

    connect(ui->saveButton, &QPushButton::clicked, canvas , &Canvas::saveFile);
    connect(ui->openButton, &QPushButton::clicked, canvas , &Canvas::openFile);

    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::showColorDialog);
    connect(this, &MainWindow::setPenColor, canvas, &Canvas::setPenColor);

    connect(canvas, &Canvas::updateCanvasDisplay, this, &MainWindow::updateCanvasDisplay);
}

void MainWindow::canvasSizeDialog()
{
    bool ok;
    int userCanvasSize = ui->inputCanvasSize->text().toInt(&ok);

    if (ok && userCanvasSize >= 1 && userCanvasSize <= 60) {
        canvas->setCanvasSize(userCanvasSize);
        QString labelSize = QString::number(userCanvasSize) + " x " + QString::number(userCanvasSize);
        ui->labelCanvasSize->setText("Canvas Size: " + labelSize);
        updateCanvasDisplay(QPixmap::fromImage(canvas->getCanvasImage().scaled(400,400)));

        ui->canvasDialog->hide();
        setEnabled(true);
    } else {
        QMessageBox::warning(this, "Invalid Input", "Please enter a value between 1 and 64.");
    }
}

void MainWindow::updateCanvasDisplay(QPixmap pixmap)
{
    ui->canvas->setPixmap(pixmap);
}

void MainWindow::updateFPS()
{
    // TODO: Did not implement min,max size for FPS
    // ...
    ui->labelFps->setText("FPS: " + QString::number(ui->slider_fps->value()));
}

void MainWindow::showColorDialog()
{
    QColor selectedColor = colorDialog.getColor();

    if (selectedColor.isValid()) {
        emit setPenColor(selectedColor);
        ui->labelColor->setStyleSheet(QString("background-color: %1").arg(selectedColor.name()));
    }
}

