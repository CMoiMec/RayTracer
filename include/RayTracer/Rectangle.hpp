/*
** Rectangle - A rectangular planar primitive in 3D space
*/
#pragma once
#include "IPrimitive.hpp"
#include "Math/Rectangle3D.hpp"
#include "Ray.hpp"
#include "Core/ITransformable.hpp"
#include <cmath>
#include <limits>

namespace RayTracer {
    /**
     * @brief A visible rectangle primitive defined by 3D position and sides.
     */
    class Rectangle : public IPrimitive, public Core::ITransformable {
        public:
            Rectangle(const Math::Point3D& origin, const Math::Vector3D& bottom, const Math::Vector3D& left,
                const Color& col=Color(255,255,255));
            ~Rectangle();

            bool hits(const Ray& ray, HitInfo& info) const override;
            Math::Rectangle3D _geometry;
            Color _color;

            void translate(const Math::Vector3D& offset) override;
            const Color& getColor() const;
        };
}
