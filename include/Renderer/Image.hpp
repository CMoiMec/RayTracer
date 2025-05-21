/*
**
**
**
**
*/

#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdint>
#include "Utils/Color.hpp"

/**
 * \brief Simple 8-bit RGB frame-buffer.
 */
class Image {
public:
    Image(int width, int height);
    ~Image() = default;

    void  setPixel(int x, int y, const Color& c);
    Color getPixel(int x, int y) const;

    /**
     * \brief Write the buffer in ASCII-PPM (P3) format.
     * \param path  Destination file path (e.g. "screenshots/out.ppm")
     */
    void writePPM(const std::string& path) const;

    int width()  const { return _width; }
    int height() const { return _height; }

    std::vector<uint8_t> toRGBA() const;

private:
    int _width;
    int _height;
    std::vector<Color> _pixels;   // row-major (y * width + x)
};
