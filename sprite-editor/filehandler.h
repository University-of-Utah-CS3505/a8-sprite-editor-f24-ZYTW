#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>
#include <vector>
#include "Pixel.h"

class FileHandler
{
public:
    FileHandler() = default;

    void saveFile(const QString& path, int framesPerSecond, int canvasSize, const std::vector<std::vector<Pixel>>& pixels);
    bool loadFile(const QString& path, int& framesPerSecond, int& canvasSize, std::vector<std::vector<Pixel>>& pixels);
};

#endif // FILEHANDLER_H
