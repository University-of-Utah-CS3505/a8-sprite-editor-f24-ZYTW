QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animationWindow.cpp \
    canvas.cpp \
    colorPalette.cpp \
    eraserTool.cpp \
    fileHandler.cpp \
    frameDisplayWindow.cpp \
    frameManager.cpp \
    main.cpp \
    mainWindow.cpp \
    penTool.cpp \
    pixel.cpp \
    shapeTool.cpp \
    stampGallery.cpp \
    symmetryTool.cpp

HEADERS += \
    animationWindow.h \
    canvas.h \
    colorPalette.h \
    eraserTool.h \
    fileHandler.h \
    frameDisplayWindow.h \
    frameManager.h \
    mainWindow.h \
    penTool.h \
    pixel.h \
    shapeTool.h \
    stampGallery.h \
    symmetryTool.h \
    tool.h

FORMS += \
    mainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
