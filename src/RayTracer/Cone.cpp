/*
**
**
**
**
*/

#include "RayTracer/Cone.hpp"
#include <initializer_list>
#include <cmath>

namespace RayTracer {

Cone::Cone(const Math::Point3D& apex, const Math::Vector3D& axis, double radius, double height)
: _apex(apex), _axis(axis.normalize()), _radius(radius), _height(height), _color(255, 255, 255)
{
    _baseCenter = _apex + _axis * _height;
}

Cone::Cone(const Math::Point3D& apex, const Math::Vector3D& axis, double radius,
           double height, const Color& color)
: _apex(apex), _axis(axis.normalize()), _radius(radius), _height(height), _color(color)
{
    _baseCenter = _apex + _axis * _height;
}

Cone::~Cone()
{}

bool RayTracer::Cone::hits(const Ray& ray, HitInfo& info) const
{
    const double EPS = 1e-8;
    Math::Vector3D axis = _axis.normalize();
    double k    = (_radius/_height)*(_radius/_height);
    double cos2 = 1.0/(1.0 + k);

    Math::Vector3D oc  = Math::Vector3D(_apex, ray._origin);
    double dv  = ray._direction.dot(axis);
    double cov = oc.dot(axis);

    double a = ray._direction.dot(ray._direction) - (1.0 + k) * dv * dv;
    double b = 2.0 * ( ray._direction.dot(oc) - (1.0 + k) * dv * cov );
    double c = oc.dot(oc)   - (1.0 + k) * cov * cov;

    double disc = b*b - 4*a*c;
    double best_t = std::numeric_limits<double>::infinity();
    Math::Vector3D bestN;
    bool hit = false;

    if (disc >= 0.0 && std::abs(a) > EPS) {
        double sq = std::sqrt(disc);
        double t0 = (-b - sq) / (2*a);
        double t1 = (-b + sq) / (2*a);
        for (double t : {t0, t1}) {
            if (t < EPS) continue;
            double h = cov + t * dv;
            if (h < 0.0 || h > _height) continue;
            Math::Point3D P = ray._origin + ray._direction * t;
            Math::Vector3D XAm = Math::Vector3D(_apex, P);
            double XAm_v = XAm.dot(axis);
            Math::Vector3D grad = axis * (2*XAm_v)
                                - XAm    * (2*cos2);
            Math::Vector3D N = grad.normalize();
            if (N.dot(ray._direction) > 0)
                N = N * -1.0;
            if (t < best_t) {
                best_t = t;
                bestN  = N;
                hit    = true;
            }
        }
    }

    Math::Point3D Cb = _apex + axis * _height;
    {
        double denom = ray._direction.dot(axis);
        if (std::abs(denom) > EPS) {
            double t2 = Math::Vector3D(ray._origin, Cb).dot(axis) / denom;
            if (t2 > EPS && t2 < best_t) {
                Math::Point3D P = ray._origin + ray._direction * t2;
                Math::Vector3D diff(Cb, P);
                if (diff.dot(diff) <= _radius*_radius) {
                    best_t = t2;
                    bestN  = (denom < 0 ? axis : axis * -1.0);
                    hit    = true;
                }
            }
        }
    }

    if (!hit)
        return false;

    info.t     = best_t;
    info.p     = ray._origin + ray._direction * best_t;
    info.n     = bestN;
    info.color = &_color;
    return true;
}


const Color& Cone::getColor() const
{
    return _color;
}

const Math::Point3D& Cone::getApex() const
{
    return _apex;
}

const Math::Vector3D& Cone::getAxis() const
{
    return _axis;
}

double Cone::getRadius() const
{
    return _radius;
}

double Cone::getHeight() const
{
    return _height;
}

const Math::Point3D& Cone::getBaseCenter() const
{
    return _baseCenter;
}

void Cone::translate(const Math::Vector3D& offset)
{
    _apex.translate(offset);
    _baseCenter.translate(offset);
}

}
