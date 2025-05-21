/*
**
**
**
**
*/

#include  "Math/Vector3D.hpp"

Math::Vector3D::Vector3D()
{
    this->_x;
    this->_y;
    this->_z;
}

Math::Vector3D::Vector3D(const Point3D& a, const Point3D& b)
{
    this->_x = b._x - a._x;
    this->_y = b._y - a._y;
    this->_z = b._z - a._z;
}

Math::Vector3D::Vector3D(double x, double y, double z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

double Math::Vector3D::length()const
{
    return sqrt(_x *_x + _y *_y + _z *_z);
}

Math::Vector3D Math::Vector3D::operator+(const Vector3D& other) const
{
    return Math::Vector3D(this->_x + other._x, this->_y + other._y, this->_z + other._z);
}

Math::Vector3D Math::Vector3D::operator-(const Vector3D& other)const
{
    return Math::Vector3D(this->_x - other._x, this->_y - other._y, this->_z - other._z);
}

Math::Vector3D Math::Vector3D::operator*(const Vector3D& other)const
{
    return Math::Vector3D(this->_x * other._x, this->_y * other._y, this->_z * other._z);
}

Math::Vector3D Math::Vector3D::operator/(const Vector3D& other)const
{
    return Math::Vector3D(this->_x / other._x, this->_y / other._y, this->_z / other._z);
}

Math::Vector3D Math::Vector3D::operator+=(const Vector3D& other)
{
    this->_x += other._x;
    this->_y += other._y;
    this->_z += other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator-=(const Vector3D& other)
{
    this->_x -= other._x;
    this->_y -= other._y;
    this->_z -= other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator*=(const Vector3D& other)
{
    this->_x *= other._x;
    this->_y *= other._y;
    this->_z *= other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator/=(const Vector3D& other)
{
    this->_x /= other._x;
    this->_y /= other._y;
    this->_z /= other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator*(double b)const
{
    return Math::Vector3D(this->_x * b, this->_y * b, this->_z * b);
}

Math::Vector3D Math::Vector3D::operator/(double b)const
{
    return Math::Vector3D(this->_x / b, this->_y / b, this->_z / b);
}


Math::Vector3D Math::Vector3D::operator*=(double b)
{
    this->_x *= b;
    this->_y *= b;
    this->_z *= b;
    return *this;
}


Math::Vector3D Math::Vector3D::operator/=(double b)
{
    this->_x /= b;
    this->_y /= b;
    this->_z /= b;
    return *this;
}

double Math::Vector3D::dot(const Vector3D& other) const
{
    return this->_x * other._x + this->_y * other._y + this->_z * other._z;
}

Math::Vector3D Math::Vector3D::cross(const Vector3D& other) const
{
    return Vector3D(
        _y * other._z - _z * other._y,
        _z * other._x - _x * other._z,
        _x * other._y - _y * other._x
    );
}

Math::Vector3D Math::Vector3D::normalize() const
{
    double len = length();
    if (len == 0)
        return Vector3D(0, 0, 0);
    return Vector3D(_x / len, _y / len, _z / len);
}

Math::Vector3D Math::Vector3D::operator-() const
{
    return Vector3D(-_x, -_y, -_z);
}

double Math::Vector3D::length2() const
{
    return _x*_x + _y*_y + _z*_z;
}

Math::Vector3D Math::Vector3D::rotateX(double angle) const
{
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);
    return Vector3D(
        _x,
        _y * cosA - _z * sinA,
        _y * sinA + _z * cosA
    );
}

Math::Vector3D Math::Vector3D::rotateY(double angle) const
{
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);
    return Vector3D(
        _x * cosA + _z * sinA,
        _y,
        -_x * sinA + _z * cosA
    );
}

Math::Vector3D Math::Vector3D::rotateZ(double angle) const
{
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);
    return Vector3D(
        _x * cosA - _y * sinA,
        _x * sinA + _y * cosA,
        _z
    );
}

