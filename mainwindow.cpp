#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Sprite& sprite, QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sprite(sprite)
{
    ui->setupUi(this);

    // Initialize UI
    updateCanvasDisplay();
    ui->canvasDialog->show();
    ui->canvasDialog->setEnabled(true);
    ui->openButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->penButton->setEnabled(false);
    ui->eraserButton->setEnabled(false);
    ui->parallelButton->setEnabled(false);
    ui->colorButton->setEnabled(false);
    ui->shapeButton->setEnabled(false);
    ui->slider_fps->setEnabled(false);
    ui->canvas->setEnabled(false);

    connect(ui->inputButton, &QPushButton::clicked, this, &MainWindow::clickedCanvasDialog);

    connect (this, &MainWindow::canvasSizeChanged, &sprite, &Sprite::setCanvasSize);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickedCanvasDialog()
{
    bool ok;
    int userCanvasSize = ui->inputCanvasSize->text().toInt(&ok);

    if (ok && userCanvasSize >=1 && userCanvasSize <= 64) {
        QString labelSize = QString::number(userCanvasSize) + " x " + QString::number(userCanvasSize);
        ui->labelCanvasSize->setText("Canvas Size: " + labelSize);

        emit canvasSizeChanged(userCanvasSize);
        updateCanvasDisplay();

        ui->canvasDialog->hide();
        setEnabled(true);
    }
    else {
        QMessageBox::warning(this, "Invalid Input", "Please enter a value between 1 and 64.");
    }

}

void MainWindow::updateCanvasDisplay() {
    ui->canvas->setPixmap(QPixmap::fromImage(sprite.getCanvasImage().scaled(400,400)));
}
