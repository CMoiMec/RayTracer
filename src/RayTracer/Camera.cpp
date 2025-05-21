/*
**
**
**
**
*/

#include "RayTracer/Camera.hpp"


RayTracer::Camera::Camera():
_screen(Math::Point3D(0,0,0), Math::Vector3D(0,1,0), Math::Vector3D(1,0,0)),
_origin(0,0,0)
{}

RayTracer::Camera::~Camera()
{}

RayTracer::Ray RayTracer::Camera::ray(double u, double v) const
{
    Math::Point3D target = _screen.pointAt(v, u);
    Math::Vector3D direction(
        target._x - _origin._x,
        target._y - _origin._y,
        target._z - _origin._z
    );
    direction = direction.normalize();
    return RayTracer::Ray(_origin, direction);
}

void RayTracer::Camera::translate(const Math::Vector3D& offset)
{
    _origin.translate(offset);
    _screen.translate(offset);
    return;
}
