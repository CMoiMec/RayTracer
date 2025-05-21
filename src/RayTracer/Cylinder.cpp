/*
**
**
**
**
*/

#include "RayTracer/Cylinder.hpp"
#include <cmath>
#include <initializer_list>

namespace RayTracer {

Cylinder::Cylinder(const Math::Point3D& baseCenter, const Math::Vector3D& axis,
                   double radius, double height)
: _baseCenter(baseCenter), _axis(axis.normalize()), _radius(radius), _height(height), _color(255, 255, 255)
{
    _topCenter = _baseCenter + _axis * _height;
}

Cylinder::Cylinder(const Math::Point3D& baseCenter, const Math::Vector3D& axis,
                   double radius, double height, const Color& color)
: _baseCenter(baseCenter), _axis(axis.normalize()), _radius(radius), _height(height), _color(color)
{
    _topCenter = _baseCenter + _axis * _height;
}

Cylinder::~Cylinder()
{}

bool Cylinder::hits(const Ray& ray, HitInfo& info) const
{
    const double EPS = 1e-8;
    Math::Vector3D v = _axis.normalize();
    Math::Vector3D D = ray._direction.normalize();
    const Math::Point3D& O = ray._origin;

    Math::Vector3D oc = Math::Vector3D(_baseCenter, O);

    Math::Vector3D D_ort = D - v * D.dot(v);
    Math::Vector3D oc_ort = oc - v * oc.dot(v);

    double a = D_ort.dot(D_ort);
    double b = 2.0 * D_ort.dot(oc_ort);
    double c = oc_ort.dot(oc_ort) - _radius*_radius;

    double best_t = std::numeric_limits<double>::infinity();
    bool hitSomething = false;
    Math::Vector3D bestNormal;

    double disc = b*b - 4*a*c;
    if (disc >= 0.0 && a > EPS) {
        double sq = std::sqrt(disc);
        double t0 = (-b - sq) / (2*a);
        double t1 = (-b + sq) / (2*a);
        for (double t : {t0, t1}) {
            if (t < EPS) continue;
            Math::Point3D P = O + D * t;
            double h = (Math::Vector3D(_baseCenter, P)).dot(v);
            if (h >= 0.0 && h <= _height) {
                if (t < best_t) {
                    best_t = t;
                    Math::Vector3D normal = Math::Vector3D(_baseCenter, P)
                                               - v * h;
                    bestNormal = normal.normalize();
                    hitSomething = true;
                }
            }
        }
    }

    {
        double denom = D.dot(v);
        if (std::abs(denom) > EPS) {
            double t2 = -( Math::Vector3D(_baseCenter, O).dot(v) ) / denom;
            if (t2 > EPS && t2 < best_t) {
                Math::Point3D P = O + D * t2;
                Math::Vector3D d = Math::Vector3D(_baseCenter, P);
                if (d.dot(d) <= _radius*_radius) {
                    best_t = t2;
                    bestNormal = (denom < 0 ? v : v * -1.0);
                    hitSomething = true;
                }
            }
        }
    }

    Math::Point3D C1 = _baseCenter + v * _height;
    {
        double denom = D.dot(v);
        if (std::abs(denom) > EPS) {
            double t3 = ( Math::Vector3D(O, C1).dot(v) ) / denom;
            if (t3 > EPS && t3 < best_t) {
                Math::Point3D P = O + D * t3;
                Math::Vector3D d = Math::Vector3D(C1, P);
                if (d.dot(d) <= _radius*_radius) {
                    best_t = t3;
                    bestNormal = (denom > 0 ? v : v * -1.0);
                    hitSomething = true;
                }
            }
        }
    }

    if (!hitSomething)
        return false;

    info.t = best_t;
    info.p = ray._origin + D * best_t;
    info.n = bestNormal;
    info.color = &_color;
    return true;
}

const Color& Cylinder::getColor() const
{
    return _color;
}

const Math::Point3D& Cylinder::getBaseCenter() const
{
    return _baseCenter;
}

const Math::Vector3D& Cylinder::getAxis() const
{
    return _axis;
}

double Cylinder::getRadius() const
{
    return _radius;
}

double Cylinder::getHeight() const
{
    return _height;
}

const Math::Point3D& Cylinder::getTopCenter() const
{
    return _topCenter;
}

void Cylinder::translate(const Math::Vector3D& offset)
{
    _baseCenter.translate(offset);
    _topCenter.translate(offset);
}

}
