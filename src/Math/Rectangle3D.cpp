/*
**
**
**
**
*/

#include   "Math/Rectangle3D.hpp"


Math::Rectangle3D::Rectangle3D() : _origin(Math::Point3D()), _bottom_side(Math::Vector3D()), _left_side(Math::Vector3D())
{}


Math::Rectangle3D::Rectangle3D(const Math::Point3D& c, const Math::Vector3D& top, const Math::Vector3D& bottom)
{
    this->_bottom_side = bottom;
    this->_left_side = top;
    this->_origin = c;
}

Math::Rectangle3D::~Rectangle3D()
{}

Math::Point3D Math::Rectangle3D::pointAt(double u, double v) const
{
    return this->_origin + (_bottom_side * u) + (_left_side * v);
}

void Math::Rectangle3D::translate(const Math::Vector3D& offset)
{
    _origin.translate(offset);
    return;
}