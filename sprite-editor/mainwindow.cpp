#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this))
    , colorPalette(new ColorPalette(this))
    , stampGallery(new StampGallery())
    , symmetryTool(new SymmetryTool())
    , shapeTool(new ShapeTool(this))
    , frameManager(new FrameManager())
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

    connect(ui->saveButton, &QPushButton::clicked, canvas , &Canvas::saveFile);
    connect(ui->openButton, &QPushButton::clicked, canvas , &Canvas::openFile);

    // Tool buttons
    connect(ui->penButton, &QPushButton::clicked, this, &MainWindow::selectPenTool);
    connect(ui->eraserButton, &QPushButton::clicked, this, &MainWindow::selectEraserTool);
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::selectPaletteTool);
    connect(this, &MainWindow::setPenColor, canvas, &Canvas::setPenColor);

    connect(canvas, &Canvas::updateCanvasDisplay, this, &MainWindow::updateCanvasDisplay);

    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setPenColor);


    //Frames buttons
    connect(ui->previewButton, &QPushButton::clicked, [&]() { frameManager->startPreview(); });
    connect(ui->addButton, &QPushButton::clicked, [&]() { frameManager->addFrame(); });
    connect(ui->removeButton, &QPushButton::clicked, [&]() { frameManager->removeFrame(selectedIndex); });
    connect(ui->fpsSlider, &QSlider::valueChanged, [&](int fps) { frameManager->setFPS(fps); });

    //Shape buttons
    QMenu *shapeMenu = new QMenu(this);
    QAction *rectAction = shapeMenu->addAction("Rectangle");
    QAction *ellipseAction = shapeMenu->addAction("Ellipse");
    QAction *triangleAction = shapeMenu->addAction("Triangle");
    connect(rectAction, &QAction::triggered, [&]() {
        shapeTool->setShape("Rectangle");
        currentTool = shapeTool;
        updateCanvasTool();
    });
    connect(ellipseAction, &QAction::triggered, [&]() {
        shapeTool->setShape("Ellipse");
        currentTool = shapeTool;
        updateCanvasTool();
    });
    connect(triangleAction, &QAction::triggered, [&]() { shapeTool->setShape("Triangle");
        currentTool = shapeTool;
        updateCanvasTool(); });

    ui->shapeButton->setMenu(shapeMenu);

    //Stamp button
    connect(ui->saveStampButton, &QPushButton::clicked, [&]() {
        QImage currentSprite = canvas->getCurrentSprite();
        stampGallery->saveStamp(currentSprite);
    });

    //Symmetry button
    connect(ui->parallelButton, &QPushButton::clicked, [&]() {
        symmetryTool->enableSymmetry(!symmetryTool->isSymmetryEnabled());
    });
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
