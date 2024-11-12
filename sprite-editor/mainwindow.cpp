#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this))
    , colorPalette(new ColorPalette(this))
{
    ui->setupUi(this);

    penTool = new Pen(Qt::black);
    eraserTool = new Eraser(Qt::white);

    currentTool = penTool;

    if (!ui->canvas->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->canvas);
        ui->canvas->setLayout(layout);
    }

    ui->canvas->layout()->addWidget(canvas);

    canvas->setTool(currentTool);

    setUpConnections();
    updateCanvasDisplay(QPixmap::fromImage(canvas->getCanvasImage().scaled(400,400)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete penTool;
    delete eraserTool;
    delete colorPalette;
}

void MainWindow::setUpConnections()
{
    connect(ui->inputButton, &QPushButton::clicked, this, &MainWindow::canvasSizeDialog);
    connect(ui->slider_fps, &QSlider::valueChanged, this, &MainWindow::updateFPS);

    connect(ui->saveButton, &QPushButton::clicked, canvas , &Canvas::saveCanvas);
    connect(ui->openButton, &QPushButton::clicked, canvas , &Canvas::loadCanvas);

    // Tool buttons
    connect(ui->penButton, &QPushButton::clicked, this, &MainWindow::selectPenTool);
    connect(ui->eraserButton, &QPushButton::clicked, this, &MainWindow::selectEraserTool);
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::selectPaletteTool);
    connect(this, &MainWindow::setPenColor, canvas, &Canvas::setPenColor);

    connect(canvas, &Canvas::updateCanvasDisplay, this, &MainWindow::updateCanvasDisplay);

    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setPenColor);
}

void MainWindow::canvasSizeDialog() {
    bool ok;
    int userCanvasSize = ui->inputCanvasSize->text().toInt(&ok);

    if (ok && userCanvasSize >= 1 && userCanvasSize <= 64) {
        canvas->setCanvasSize(userCanvasSize);
        QString labelSize = QString::number(userCanvasSize * 10) + " x " + QString::number(userCanvasSize * 10);
        ui->labelCanvasSize->setText("Canvas Size: " + labelSize);

        updateCanvasDisplay(QPixmap::fromImage(canvas->getCanvasImage().scaled(400, 400)));
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

void MainWindow::selectPaletteTool()
{
    colorPalette->openColorDialog();
}

void MainWindow::setPenColor(QColor color) {
    penTool->setColor(color);
    ui->labelColor->setStyleSheet(QString("background-color: %1").arg(color.name()));
}

void MainWindow::selectPenTool() {
    currentTool = penTool;
    updateCanvasTool();
}

void MainWindow::selectEraserTool() {
    currentTool = eraserTool;
    updateCanvasTool();
}

void MainWindow::updateCanvasTool() {
    if (canvas) {
        canvas->setTool(currentTool);
    }
}
