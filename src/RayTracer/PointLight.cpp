/*
**
**
**
**
*/

#include "RayTracer/PointLight.hpp"
#include <algorithm>

namespace RayTracer {

PointLight::PointLight(const Math::Point3D& position, const Color& col, double intensity)

: m_position(position), m_intensity(std::clamp(intensity, 0.0, 1.0)), m_color(col)
{}

const Math::Point3D& PointLight::getPosition() const
{
    return m_position;
}

double PointLight::getIntensity() const
{
    return m_intensity;
}

Color PointLight::getColor() const
{
    return m_color;
}
void PointLight::translate(const Math::Vector3D& offset)
{
    m_position.translate(offset);
    return;
}
}
