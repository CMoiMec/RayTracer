/*
** Cylinder - A finite cylinder primitive in 3D space
**
** Represents a cylinder defined by a base center point, an axis direction,
** a radius and a height. Includes intersection testing for ray tracing
** and supports geometric transformations.
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
     * @brief A finite cylinder primitive defined by base center, axis direction, radius and height.
     */
    class Cylinder : public IPrimitive, public Core::ITransformable {
        private:
            Math::Point3D _baseCenter;   // Center of the bottom circular face
            Math::Vector3D _axis;        // Direction from base to top
            double _radius;              // Radius of the cylinder
            double _height;              // Height of the cylinder
            Color _color;                // Color of the cylinder
            Math::Point3D _topCenter;    // Center of the top circular face

        public:
            Cylinder(const Math::Point3D& baseCenter, const Math::Vector3D& axis,
                     double radius, double height);
            Cylinder(const Math::Point3D& baseCenter, const Math::Vector3D& axis, double radius,
                    double height, const Color& color);
            ~Cylinder();

            bool hits(const Ray& ray, HitInfo& info) const override;
            const Color& getColor() const override;

            const Math::Point3D& getBaseCenter() const;
            const Math::Vector3D& getAxis() const;
            double getRadius() const;
            double getHeight() const;
            const Math::Point3D& getTopCenter() const;

            void translate(const Math::Vector3D& offset) override;
    };
}
