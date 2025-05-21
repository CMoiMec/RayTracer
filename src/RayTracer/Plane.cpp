/*
**
**
**
**
*/

#include "RayTracer/Plane.hpp"

RayTracer::Plane::Plane(const Math::Point3D& pos, const Math::Vector3D& norm)
    : _position(pos), _normal(norm.normalize()), _color(255, 255, 255)
{}

RayTracer::Plane::Plane(const Math::Point3D& pos, const Math::Vector3D& norm, const Color& color)
    : _position(pos), _normal(norm.normalize()), _color(color)
{}

RayTracer::Plane::~Plane()
{}

bool RayTracer::Plane::hits(const Ray& ray, HitInfo& hit) const
{
    const double EPSILON = 1e-8;

    double denom = _normal.dot(ray._direction);
    if (std::abs(denom) < EPSILON)
        return false;

    Math::Vector3D oc(_position, ray._origin);
    double t = -_normal.dot(oc) / denom;

    if (t < 1e-4)
        return false;

    hit.t     = t;
    hit.p     = ray._origin + ray._direction * t;
    hit.n     = (denom < 0) ? _normal : _normal * -1.0;
    hit.color = &_color;
    return true;
}

const Math::Point3D& RayTracer::Plane::getPosition() const
{
    return _position;
}

const Math::Vector3D& RayTracer::Plane::getNormal() const
{
    return _normal;
}

const Color& RayTracer::Plane::getColor() const
{
    return _color;
}

void RayTracer::Plane::translate(const Math::Vector3D& offset)
{
    _position.translate(offset);
    return;
}
