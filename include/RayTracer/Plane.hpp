/*
** Plane - An infinite planar surface in 3D space
*/
#pragma once
#include "IPrimitive.hpp"
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"
#include "Ray.hpp"
#include "Utils/Color.hpp"
#include "Core/ITransformable.hpp"

namespace RayTracer {
    /**
     * @brief An infinite plane primitive defined by a point and a normal.
     */
    class Plane : public IPrimitive, public Core::ITransformable {
        private:
            Math::Point3D _position;     // A point on the plane
            Math::Vector3D _normal;      // Normal vector perpendicular to the plane
            Color _color;                // Color of the plane

        public:
            Plane(const Math::Point3D& pos, const Math::Vector3D& norm);
            Plane(const Math::Point3D& pos, const Math::Vector3D& norm, const Color& color);
            ~Plane();

            bool hits(const Ray& ray, HitInfo &info) const override;

            const Math::Point3D& getPosition() const;
            const Math::Vector3D& getNormal() const;
            const Color& getColor() const;
            void translate(const Math::Vector3D& offset) override;
        };
}
