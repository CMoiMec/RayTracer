/*
**
**
**
**
*/

#pragma once

#include <cmath>

namespace Math {
    class Point3D;
    class Vector3D {
        private:
        public:
            double _x;
            double _y;
            double _z;
            Vector3D();
            Vector3D(const Point3D& a, const Point3D& b);
            Vector3D(double x, double y, double z);
            ~Vector3D() = default;
            Vector3D operator+(const Vector3D& other)const;
            Vector3D operator-(const Vector3D& other)const;
            Vector3D operator*(const Vector3D& other)const;
            Vector3D operator/(const Vector3D& other)const;
            Vector3D operator+=(const Vector3D& other);
            Vector3D operator-=(const Vector3D& other);
            Vector3D operator*=(const Vector3D& other);
            Vector3D operator/=(const Vector3D& other);
            Vector3D operator*(double d)const;
            Vector3D operator*=(double d);
            Vector3D operator/(double d)const;
            Vector3D operator/=(double d);
            Vector3D operator-() const;
            /**
             * @brief Rotate this vector around the X axis by the given angle (radians).
             */
            Vector3D rotateX(double angle) const;

            /**
             * @brief Rotate this vector around the Y axis by the given angle (radians).
             */
            Vector3D rotateY(double angle) const;

            /**
             * @brief Rotate this vector around the Z axis by the given angle (radians).
             */
            Vector3D rotateZ(double angle) const;


            double length() const;
            double dot(const Vector3D& other) const;
            Vector3D cross(const Vector3D& other) const;
            Vector3D normalize() const;
            double length2() const;

    };
}

#include "Math/Point3D.hpp"
