/*
**
**
**
**
*/

#include   "Math/Point3D.hpp"


Math::Point3D::Point3D()
{
    this->_x = 0;
    this->_y = 0;
    this->_z = 0;
}

Math::Point3D::Point3D(double x, double y, double z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

Math::Point3D::~Point3D()
{

}

Math::Point3D Math::Point3D::operator+(const Vector3D& other)const
{
    return Point3D(this->_x + other._x, this->_y + other._y, this->_z + other._z);
}


Math::Point3D Math::Point3D::operator-(const Vector3D& other)const
{
    return Point3D(this->_x - other._x, this->_y - other._y, this->_z - other._z);
}

Math::Point3D Math::Point3D::operator+=(const Vector3D& other)
{
    this->_x += other._x;
    this->_y += other._y;
    this->_z += other._z;
    return *this;
}

Math::Point3D Math::Point3D::operator-=(const Vector3D& other)
{
    this->_x -= other._x;
    this->_y -= other._y;
    this->_z -= other._z;
    return *this;
}

void Math::Point3D::translate(const Math::Vector3D& offset)
{
    (*this) += offset;
    return;
}