/*
**
**
**
**
*/

#pragma once

#include "Vector3D.hpp"
#include "Core/ITransformable.hpp"

namespace Math {
    class Point3D : public Core::ITransformable {
        public:
            double _x;
            double _y;
            double _z;
            Point3D();
            Point3D(double x, double y, double z);
            ~Point3D();
            Point3D operator+(const Vector3D& other)const;
            Point3D operator-(const Vector3D& other)const;
            Point3D operator+=(const Vector3D& other);
            Point3D operator-=(const Vector3D& other);

            void translate(const Vector3D& offset) override;
    };
}