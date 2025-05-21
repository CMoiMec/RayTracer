/*
**
**
**
**
*/

#include "RayTracer/DirectionalLight.hpp"
#include <algorithm>

namespace RayTracer {

DirectionalLight::DirectionalLight(const Math::Vector3D& direction, const Color& col,double intensity)
    : m_direction(direction.normalize()), m_intensity(std::clamp(intensity, 0.0, 1.0)), _color(col)
{}

const Math::Vector3D& DirectionalLight::getDirection() const
{
    return m_direction;
}

double DirectionalLight::getIntensity() const
{
    return m_intensity;
}

Color DirectionalLight::getColor() const
{
    return _color;
}

}
