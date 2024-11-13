#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this))
    , colorPalette(new ColorPalette(this))
    , stampGallery(new StampGallery())
    , symmetryTool(new SymmetryTool())
    , frameManager(new FrameManager())
{
    ui->setupUi(this);

    penTool = new Pen(Qt::black);
    eraserTool = new Eraser(Qt::white);
    shapeTool = new ShapeTool(this, "Rectangle");
    symmetryTool = new SymmetryTool(this);

    currentTool = penTool;

    animationWindow = ui->animationWindow;
    connect(frameManager, &FrameManager::frameChanged, ui->animationWindow, &AnimationWindow::displayFrame);


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
    connect(ui->penButton, &QPushButton::clicked, [&]() {
        currentTool = penTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->penButton);
    });
    connect(ui->eraserButton, &QPushButton::clicked, [&]() {
        currentTool = eraserTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->eraserButton);
    });
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::selectPaletteTool);
    connect(this, &MainWindow::setPenColor, canvas, &Canvas::setPenColor);

    connect(canvas, &Canvas::updateCanvasDisplay, this, &MainWindow::updateCanvasDisplay);

    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setPenColor);
    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setShapeColor);
    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setSymmetryColor);


    //Frames buttons
    connect(ui->previewButton, &QPushButton::clicked, this, [&]() {
        if (previewActive) {
            frameManager->stopPreview();
            ui->previewButton->setText("Start Preview");
            animationWindow->hide();  // Hide the animation window when stopping preview
        } else {
            animationWindow->show();  // Ensure the animation window is shown
            frameManager->startPreview();
            ui->previewButton->setText("Stop Preview");
        }
        previewActive = !previewActive;
    });
    connect(ui->addFrameButton, &QPushButton::clicked, [&]() {
        QImage currentCanvasImage = canvas->getCanvasImage();  // Capture current canvas content
        frameManager->addFrame(currentCanvasImage);  // Add it as a new frame
        selectedIndex = frameManager->getFrames().size() - 1;
    });
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, [&]() {
        int frameToDelete = selectedIndex;  // Use the selected index or specify an index manually
        frameManager->removeFrame(frameToDelete);

        // Update the UI or selected index as needed
        selectedIndex = qMax(0, selectedIndex - 1);  // Adjust selectedIndex to stay in bounds
    });
    connect(ui->fpsSlider, &QSlider::valueChanged, [&](int fps) {
        frameManager->setFPS(fps);
        ui->labelFps->setText("FPS: " + QString::number(fps));
    });



    //Shape buttons
    QMenu *shapeMenu = new QMenu(this);
    QAction *rectAction = shapeMenu->addAction("Rectangle");
    QAction *ellipseAction = shapeMenu->addAction("Ellipse");
    QAction *triangleAction = shapeMenu->addAction("Triangle");
    connect(rectAction, &QAction::triggered, [&]() {
        shapeTool->setShapeType("Rectangle");
        currentTool = shapeTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->shapeButton);
    });

    connect(ellipseAction, &QAction::triggered, [&]() {
        shapeTool->setShapeType("Ellipse");
        currentTool = shapeTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->shapeButton);
    });

    connect(triangleAction, &QAction::triggered, [&]() {
        shapeTool->setShapeType("Triangle");
        currentTool = shapeTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->shapeButton);
    });

    ui->shapeButton->setMenu(shapeMenu);

    //StampGallery button
    connect(ui->selectStampButton, &QPushButton::clicked, this, &MainWindow::onSelectStampButtonClicked);
    connect(ui->saveStampButton, &QPushButton::clicked, [&]() {
        auto selectedPixels = canvas->getSelectedPixels();
        if (!selectedPixels.empty()) {
            stampGallery->saveStamp(selectedPixels);
        } else {
            qWarning("No pixels selected for saving the stamp");
        }
    });
    connect(ui->loadStampButton, &QPushButton::clicked, [&]() {
         stampGallery->loadStampsFromResource();
         stampGallery->show();
    });

    //Symmetry button
    connect(ui->parallelButton, &QPushButton::clicked, [&]() {
        currentTool = symmetryTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->parallelButton);
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
        canvas->update();
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

void MainWindow::selectShapeTool() {
    shapeTool->setShapeType("Rectangle");
    currentTool = shapeTool;
     updateCanvasTool();
}

void MainWindow::setShapeColor(QColor color) {
    if (shapeTool) {
        shapeTool->setColor(color);
    }
}

void MainWindow::setSymmetryColor(QColor color) {
    if (symmetryTool) {
        symmetryTool->setColor(color);
    }
}

void MainWindow::updateToolButtonHighlight(QPushButton* selectedButton) {
    // Reset all tool buttons to their default state
    ui->penButton->setStyleSheet("");
    ui->eraserButton->setStyleSheet("");
    ui->shapeButton->setStyleSheet("");
    ui->parallelButton->setStyleSheet("");

    // Highlight the selected button
    if (selectedButton) {
        selectedButton->setStyleSheet("background-color: yellow;");
    }
}

void MainWindow::applyStampToCanvas(const QJsonObject& stampJson) {
    canvas->setCurrentStamp(stampJson);  // Set the selected stamp data

    QPoint cursorPos = canvas->mapFromGlobal(QCursor::pos());
    int x = cursorPos.x() / 10;
    int y = cursorPos.y() / 10;

    canvas->applyStamp(stampJson, 0, 0);
}

void MainWindow::onSelectStampButtonClicked() {
    canvas->startSelectingStamp();
}

void MainWindow::displayFrame(const QImage &frame) {
    ui->canvas->setPixmap(QPixmap::fromImage(frame).scaled(ui->canvas->size()));
}

void MainWindow::updateCanvasTool() {
    if (canvas) {
        canvas->setTool(currentTool);
    }
}
