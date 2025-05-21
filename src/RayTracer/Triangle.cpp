/*
**
**
**
**
*/

#include "RayTracer/Triangle.hpp"

RayTracer::Triangle::Triangle()
{
}

RayTracer::Triangle::Triangle(const Math::Point3D& a, const Math::Point3D& b, const Math::Point3D& c, const Color& col)
: _color(col)
{
    this->_a = a;
    this->_b = b;
    this->_c = c;
}

RayTracer::Triangle::~Triangle()
{}

bool RayTracer::Triangle::hits(const Ray& ray, HitInfo& hit) const
{
    const double EPSILON = 1e-8;

    Math::Vector3D edge1(_a, _b);
    Math::Vector3D edge2(_a, _c);

    Math::Vector3D pvec = ray._direction.cross(edge2);
    double det = edge1.dot(pvec);
    if (std::abs(det) < EPSILON)
        return false;

    double invDet = 1.0 / det;

    Math::Vector3D tvec(_a, ray._origin);
    double u = invDet * tvec.dot(pvec);
    if (u < 0.0 || u > 1.0)
        return false;

    Math::Vector3D qvec = tvec.cross(edge1);
    double v = invDet * ray._direction.dot(qvec);
    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = invDet * edge2.dot(qvec);
    if (t < EPSILON)
        return false;

    hit.t     = t;
    hit.p     = ray._origin + ray._direction * t;
    Math::Vector3D n = edge1.cross(edge2).normalize();
    hit.n     = (det < 0.0) ? n * -1.0 : n;
    hit.color = &_color;

    return true;
}

void RayTracer::Triangle::translate(const Math::Vector3D& offset)
{
    _a.translate(offset);
    _b.translate(offset);
    _c.translate(offset);
    return;
}

const Color& RayTracer::Triangle::getColor() const
{
    return this->_color;
}