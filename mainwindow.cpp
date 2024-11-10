#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Canvas.h"
#include <QMessageBox>

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
    updateCanvasSizeLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpConnections() {
    connect(ui->inputButton, &QPushButton::clicked, this, &MainWindow::canvasSizeConfirmation);
}

void MainWindow::canvasSizeConfirmation() {
    bool ok;
    int userCanvasSize = ui->inputCanvasSize->text().toInt(&ok);

    if (ok && userCanvasSize >= 1 && userCanvasSize <= 64) {
        canvas->setCanvasSize(userCanvasSize);
        updateCanvasSizeLabel();
        ui->canvasDialog->hide();
    } else {
        QMessageBox::warning(this, "Invalid Input", "Please enter a value between 1 and 64.");
    }
}

void MainWindow::updateCanvasSizeLabel() {
    QString sizeText = QString("Canvas Size: %1 x %2").arg(canvas->getCanvasSize()).arg(canvas->getCanvasSize());
    ui->labelCanvasSize->setText(sizeText);
}
