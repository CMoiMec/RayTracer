#include "Utils/Color.hpp"
#include <algorithm>

Color::Color() : m_r(0), m_g(0), m_b(0)
{}

Color::Color(int r, int g, int b)
    : m_r(std::clamp(r, 0, 255))
    , m_g(std::clamp(g, 0, 255))
    , m_b(std::clamp(b, 0, 255))
{}

int Color::getR() const
{
    return m_r;
}

int Color::getG() const
{
    return m_g;
}

int Color::getB() const
{
    return m_b;
}

void Color::setR(int r)
{
    m_r = std::clamp(r, 0, 255);
}

void Color::setG(int g)
{
    m_g = std::clamp(g, 0, 255);
}

void Color::setB(int b)
{
    m_b = std::clamp(b, 0, 255);
}

Color Color::operator+(const Color& other) const
{
    return Color(
        std::clamp(m_r + other.m_r, 0, 255),
        std::clamp(m_g + other.m_g, 0, 255),
        std::clamp(m_b + other.m_b, 0, 255)
    );
}

Color Color::operator*(float factor) const
{
    return Color(
        std::clamp(static_cast<int>(m_r * factor), 0, 255),
        std::clamp(static_cast<int>(m_g * factor), 0, 255),
        std::clamp(static_cast<int>(m_b * factor), 0, 255)
    );
}

Color& Color::operator+=(const Color& other)
{
    m_r = std::clamp(m_r + other.m_r, 0, 255);
    m_g = std::clamp(m_g + other.m_g, 0, 255);
    m_b = std::clamp(m_b + other.m_b, 0, 255);
    return *this;
}

Color& Color::operator*=(float factor)
{
    m_r = std::clamp(static_cast<int>(m_r * factor), 0, 255);
    m_g = std::clamp(static_cast<int>(m_g * factor), 0, 255);
    m_b = std::clamp(static_cast<int>(m_b * factor), 0, 255);
    return *this;
}

namespace {
    static int clampByte(int v) {
        return std::clamp(v, 0, 255);
    }
}

Color Color::operator*(const Color& o) const
{
    return Color(
        clampByte(static_cast<int>(std::round(m_r * o.m_r / 255.0f))),
        clampByte(static_cast<int>(std::round(m_g * o.m_g / 255.0f))),
        clampByte(static_cast<int>(std::round(m_b * o.m_b / 255.0f)))
    );
}

Color::Float::Float()
    : r(0.f)
    , g(0.f)
    , b(0.f) 
{}

Color::Float::Float(float r, float g, float b) : r(r), g(g), b(b) {}

Color::Float::Float(const Color& c)
    : r(c.getR() / 255.f)
    , g(c.getG() / 255.f)
    , b(c.getB() / 255.f)
{}

Color::Float& Color::Float::operator+=(const Color::Float& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color::Float Color::Float::operator*(float factor) const {
    return Color::Float(r * factor, g * factor, b * factor);
}

Color Color::Float::toColor() const {
    return Color(
        static_cast<int>(std::round(std::clamp(r * 255.f, 0.f, 255.f))),
        static_cast<int>(std::round(std::clamp(g * 255.f, 0.f, 255.f))),
        static_cast<int>(std::round(std::clamp(b * 255.f, 0.f, 255.f))));
}
