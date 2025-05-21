/*
**
**
**
**
*/

#include "RayTracer/AmbientLight.hpp"
#include <algorithm>

namespace RayTracer {

AmbientLight::AmbientLight(double intensity, const Color& col)
    : m_intensity(std::clamp(intensity, 0.0, 1.0)), m_color(col)
{}

double AmbientLight::getIntensity() const {
    return m_intensity;
}

Color AmbientLight::getColor() const
{
    return m_color;
}

}
