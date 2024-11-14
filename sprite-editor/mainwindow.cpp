#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this))
    , colorPalette(new ColorPalette(this))
    , symmetryTool(new SymmetryTool())
    , stampGallery(new StampGallery())
    , frameManager(new FrameManager())
{
    ui->setupUi(this);

    penTool = new Pen(Qt::black);
    eraserTool = new Eraser(Qt::white);
    shapeTool = new ShapeTool(this, "Rectangle");
    symmetryTool = new SymmetryTool(this);

    frameDisplayWindow = ui->frameDisplayWindow;
    currentTool = nullptr;

    animationWindow = ui->animationWindow;

    connect(ui->spinBoxControl, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value) {
        selectedIndex = value;
    });
    connect(ui->frameListWidget, &QListWidget::currentRowChanged, this, [&](int row) {
        selectedIndex = row + 1;  // Convert 0-based row to 1-based index
        ui->spinBoxControl->setValue(selectedIndex);  // Update spin box to match selection
        if (row >= 0 && row < frameManager->getFrames().size()) {
            QImage selectedFrame = frameManager->getFrames().at(row);
            ui->frameDisplayWindow->setFrameImage(selectedFrame);  // Display the frame in frameDisplayWindow
        }
    });

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
    //*********************************************************************//

    // Color Palette
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::selectPaletteTool);
    connect(this, &MainWindow::setPenColor, canvas, &Canvas::setPenColor);

    connect(canvas, &Canvas::updateCanvasDisplay, this, &MainWindow::updateCanvasDisplay);

    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setPenColor);
    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setShapeColor);
    connect(colorPalette, &ColorPalette::colorSelected, this, &MainWindow::setSymmetryColor);
    //*********************************************************************//

    //Frames buttons
    connect(ui->previewButton, &QPushButton::clicked, this, [&]() {
        if (previewActive) {
            // Stop the preview and clear animationWindow
            frameManager->stopPreview();
            ui->previewButton->setText("Start Preview");

            // Disconnect frameChanged signal from animationWindow's displayFrame slot
            disconnect(frameManager, &FrameManager::frameChanged, animationWindow, &AnimationWindow::displayFrame);

            // Clear the animationWindow display when preview stops
            animationWindow->displayFrame(QImage());  // Set to an empty QImage to clear display
        } else {
            // Start the preview and connect frameChanged signal to animationWindow
            frameManager->startPreview();
            ui->previewButton->setText("Stop Preview");

            // Connect frameChanged signal to animationWindow's displayFrame slot
            connect(frameManager, &FrameManager::frameChanged, animationWindow, &AnimationWindow::displayFrame);
        }
        previewActive = !previewActive;
    });
    connect(ui->addFrameButton, &QPushButton::clicked, this, [&]() {
        QImage newFrame = canvas->getCanvasImage();  // Capture current canvas content as new frame
        frameManager->addFrame(newFrame);  // Add the new frame to frameManager

        // Add a new item to frameListWidget with the correct label
        int newFrameIndex = frameManager->getFrames().size();
        QListWidgetItem *newItem = new QListWidgetItem("Frame " + QString::number(newFrameIndex));
        ui->frameListWidget->addItem(newItem);

        // Set the new frame as the current frame in frameDisplayWindow only
        ui->frameDisplayWindow->setFrameImage(newFrame);

        // Update spinbox maximum to reflect the new total frame count
        ui->spinBoxControl->setMaximum(newFrameIndex);
        ui->spinBoxControl->setValue(newFrameIndex);  // Set the spinbox to the newly added frame
        selectedIndex = newFrameIndex;
    });
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, [&]() {
        int frameToDelete = selectedIndex - 1;  // Convert 1-based to 0-based
        if (frameToDelete >= 0 && frameToDelete < frameManager->getFrames().size()) {
            frameManager->removeFrame(frameToDelete);

            // Add deleted frame to deletedFrameListWidget
            int originalFrameNumber = frameToDelete + 1;
            QListWidgetItem *deletedItem = new QListWidgetItem("Deleted Frame " + QString::number(originalFrameNumber));
            deletedItem->setData(Qt::UserRole, originalFrameNumber);
            ui->deletedFrameListWidget->addItem(deletedItem);

            // Update frameListWidget
            delete ui->frameListWidget->takeItem(frameToDelete);
            for (int i = 0; i < ui->frameListWidget->count(); ++i) {
                ui->frameListWidget->item(i)->setText("Frame " + QString::number(i + 1));
            }

            // Adjust spinbox maximum and selected index
            int frameCount = frameManager->getFrames().size();
            ui->spinBoxControl->setMaximum(qMax(1, frameCount));
            selectedIndex = qMin(selectedIndex, frameCount);
            ui->spinBoxControl->setValue(selectedIndex);

            // Update frameDisplayWindow if frames remain
            if (!frameManager->getFrames().isEmpty()) {
                int nextFrameIndex = qMax(0, selectedIndex - 1);
                ui->frameDisplayWindow->setFrameImage(frameManager->getFrames().at(nextFrameIndex));
            } else {
                ui->frameDisplayWindow->setFrameImage(QImage());
            }
        } else {
            qDebug() << "Invalid deletion index" << frameToDelete;
        }
    });
    connect(ui->restoreFrameButton, &QPushButton::clicked, this, [&]() {
        QListWidgetItem *selectedItem = ui->deletedFrameListWidget->currentItem();
        if (selectedItem) {
            int originalIndex = selectedItem->data(Qt::UserRole).toInt();
            auto deletedFrames = frameManager->getDeletedFrames();

            auto it = std::find_if(deletedFrames.begin(), deletedFrames.end(),
                                    [&](const QPair<int, QImage> &pair) {
                                        return pair.first == originalIndex;
                                    });

            if (it != deletedFrames.end()) {
                const QImage &deletedFrameImage = it->second;
                frameManager->restoreFrame(originalIndex, deletedFrameImage);

                // Insert restored frame into frameListWidget at the correct position
                int insertPosition = (originalIndex > ui->frameListWidget->count()) ? ui->frameListWidget->count() : originalIndex - 1;
                QListWidgetItem *restoredItem = new QListWidgetItem("Frame " + QString::number(insertPosition + 1));                    ui->frameListWidget->insertItem(insertPosition, restoredItem);

                // Remove from deletedFrameListWidget
                delete ui->deletedFrameListWidget->takeItem(ui->deletedFrameListWidget->row(selectedItem));

                // Renumber frameListWidget to maintain sequence
                for (int i = 0; i < ui->frameListWidget->count(); ++i) {
                    ui->frameListWidget->item(i)->setText("Frame " + QString::number(i + 1));
                }

                // Adjust spinbox maximum and selected index
                ui->spinBoxControl->setMaximum(ui->frameListWidget->count());
                selectedIndex = insertPosition + 1;
                ui->spinBoxControl->setValue(selectedIndex);
            } else {
                qDebug() << "Could not find frame in deletedFrames for restoration.";
             }
        }
    });
    connect(ui->deletedFrameListWidget, &QListWidget::itemClicked, this, [&](QListWidgetItem *item) {
        int originalIndex = item->data(Qt::UserRole).toInt();  // Retrieve the original index
        auto deletedFrames = frameManager->getDeletedFrames();
        for (const auto &deletedFrame : deletedFrames) {
            if (deletedFrame.first == originalIndex) {
                ui->frameDisplayWindow->setFrameImage(deletedFrame.second);  // Show the deleted frame in frameDisplayWindow
                break;
            }
        }
    });
    connect(ui->fpsSlider, &QSlider::valueChanged, [&](int fps) {
        frameManager->setFPS(fps);
        ui->labelFps->setText("FPS: " + QString::number(fps));
    });
    //*********************************************************************//

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
    //*********************************************************************//

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
         stampGallery->loadStamps();
         stampGallery->show();
    });
    connect(stampGallery, &StampGallery::stampSelected, canvas, &Canvas::setStamp);
    connect(stampGallery, &StampGallery::stampSelected, this, &MainWindow::updateStampPreview);
    //*********************************************************************//

    //Symmetry button
    connect(ui->parallelButton, &QPushButton::clicked, [&]() {
        currentTool = symmetryTool;
        canvas->setTool(currentTool);
        updateToolButtonHighlight(ui->parallelButton);
    });

    connect(canvas, &Canvas::toolCleared, this, [this]() {
        updateToolButtonHighlight(nullptr);  // Clear button highlights when no tool is active
    });
    //*********************************************************************//
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

void MainWindow::selectPaletteTool()
{
    colorPalette->openColorDialog();
}

void MainWindow::setPenColor(QColor color) {
    penTool->setColor(color);
    ui->labelColor->setStyleSheet(QString("background-color: %1").arg(color.name()));
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

void MainWindow::updateStampPreview(const QImage &stamp) {
    if (!stamp.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(stamp).scaled(
            ui->stampPreviewLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );

        ui->stampPreviewLabel->setPixmap(pixmap);
        ui->stampPreviewLabel->setAlignment(Qt::AlignCenter);  // Ensure it's centered
    } else {
        ui->stampPreviewLabel->clear();
    }
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
