/*
**
**
**
**
*/

#include "RayTracer/Rectangle.hpp"

RayTracer::Rectangle::Rectangle(const Math::Point3D& origin, const Math::Vector3D& bottom, const Math::Vector3D& left, const Color& col)
    : _geometry(origin, bottom, left), _color(col)
{
}

RayTracer::Rectangle::~Rectangle()
{}

bool RayTracer::Rectangle::hits(const Ray& ray, HitInfo& hit) const
{
    const double EPSILON = 1e-8;

    const Math::Point3D&  origin  = _geometry._origin;
    const Math::Vector3D& u       = _geometry._bottom_side;
    const Math::Vector3D& v       = _geometry._left_side;
    Math::Vector3D        normal  = u.cross(v).normalize();

    double denom = normal.dot(ray._direction);
    if (std::abs(denom) < EPSILON)
        return false;

    Math::Vector3D   P0O(origin, ray._origin);
    double           t = -normal.dot(P0O) / denom;
    if (t < EPSILON)
        return false;

    Math::Point3D    pHit = ray._origin + ray._direction * t;

    Math::Vector3D   OP(origin, pHit);
    double           uCoord = OP.dot(u) / u.dot(u);
    double           vCoord = OP.dot(v) / v.dot(v);

    if (uCoord < 0.0 || uCoord > 1.0 ||
        vCoord < 0.0 || vCoord > 1.0)
    {
        return false;
    }

    hit.t     = t;
    hit.p     = pHit;
    hit.n     = (denom < 0.0) ? normal : normal * -1.0;
    hit.color = &_color;

    return true;
}


void RayTracer::Rectangle::translate(const Math::Vector3D& offset)
{
    _geometry.translate(offset);
    return;
}

const Color& RayTracer::Rectangle::getColor() const
{
    return this->_color;
}

