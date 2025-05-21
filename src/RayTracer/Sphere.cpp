/*
**
**
**
**
*/

#include "RayTracer/Sphere.hpp"

RayTracer::Sphere::Sphere(const Math::Point3D &c, double r)
{
    this->_center = c;
    this->_radius = r;
    this->_color = Color(255, 255, 255);  // BLANC DE BLANC
}

RayTracer::Sphere::Sphere(const Math::Point3D &c, double r, const Color &color)
{
    this->_center = c;
    this->_radius = r;
    this->_color = color;
}

RayTracer::Sphere::~Sphere()
{
}

bool RayTracer::Sphere::hits(const Ray& r, HitInfo& hit) const
{
    Math::Vector3D oc(_center, r._origin);
    double a = r._direction.dot(r._direction);
    double b = 2.0 * oc.dot(r._direction);
    double c = oc.dot(oc) - _radius * _radius;
    double disc = b*b - 4*a*c;
    if (disc < 0.0) return false;

    double root = std::sqrt(disc);
    double inv2a = 1.0 / (2*a);
    double t = (-b - root) * inv2a;
    if (t < 1e-4) {
        t = (-b + root) * inv2a;
        if (t < 1e-4) return false;
    }
    hit.t     = t;
    hit.p     = r._origin + r._direction * t;
    hit.n     = Math::Vector3D(_center, hit.p).normalize();
    hit.color = &_color;
    return true;
}

const Math::Point3D& RayTracer::Sphere::getCenter() const
{
    return this->_center;
}

double RayTracer::Sphere::getRadius() const
{
    return this->_radius;
}

const Color& RayTracer::Sphere::getColor() const
{
    return this->_color;
}

void RayTracer::Sphere::translate(const Math::Vector3D& offset)
{
    _center.translate(offset);
    return;
}

