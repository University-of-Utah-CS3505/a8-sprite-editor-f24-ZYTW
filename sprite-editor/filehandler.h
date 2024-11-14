/**
 * Name: WYNTER KIM, TERESA PARK, YINHAO CHEN, ZHENGXI ZHANG
 * Course: CS 3505 Fall2024
 * Assignment Name: A8: Sprite Editor Implementation
 * Descrption: In this project, we create an intuitive and versatile pixel art tool designed for creating and editing sprites.
 *
 * Reviewer: WYNTER KIM
 */
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
