/*
**
**
**
**
*/

#include "Renderer/Image.hpp"

Image::Image(int width, int height)
    : _width(width), _height(height), _pixels(width * height, Color{})
{}

void Image::setPixel(int x, int y, const Color& c)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        return;                                // silently ignore out-of-range
    _pixels[y * _width + x] = c;
}

Color Image::getPixel(int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        throw std::out_of_range("Image::getPixel");
    return _pixels[y * _width + x];
}

void Image::writePPM(const std::string& path) const
{
    std::ofstream out(path, std::ios::out | std::ios::trunc);
    if (!out)
        throw std::runtime_error("Cannot open file for writing: " + path);

    out << "P3\n" << _width << ' ' << _height << "\n255\n";
    for (const Color& col : _pixels)
        out << static_cast<int>(col.getR()) << ' '
            << static_cast<int>(col.getG()) << ' '
            << static_cast<int>(col.getB()) << '\n';
}

std::vector<uint8_t> Image::toRGBA() const {
    std::vector<uint8_t> data;
    data.reserve(_width * _height * 4);

    auto clamp = [](int v) -> uint8_t {
        if (v < 0) return 0;
        if (v > 255) return 255;
        return static_cast<uint8_t>(v);
    };

    for (const auto& px : _pixels) {
        data.push_back(clamp(px.getR()));
        data.push_back(clamp(px.getG()));
        data.push_back(clamp(px.getB()));
        data.push_back(255);
    }
    return data;
}