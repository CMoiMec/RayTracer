/*
**
**
**
**
*/

#pragma once

#include "Point3D.hpp"
#include "Vector3D.hpp"
#include "Core/ITransformable.hpp"

namespace Math {
    class Rectangle3D : public Core::ITransformable {
        public:
            Rectangle3D();
            Rectangle3D(const Math::Point3D& c, const Math::Vector3D& top, const Math::Vector3D& bottom);
            ~Rectangle3D();
    
            Math::Point3D _origin;
            Math::Vector3D _bottom_side;
            Math::Vector3D _left_side;
            Math::Point3D pointAt(double u, double v) const;
            void translate(const Math::Vector3D& offset) override;
    };
}
