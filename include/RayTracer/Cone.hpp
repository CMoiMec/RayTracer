/*
** Cone - A finite cone primitive in 3D space
**
** Represents a cone defined by an apex point, an axis direction,
** a base radius and a height. Includes intersection logic for ray tracing
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
     * @brief A finite cone primitive defined by apex, axis direction, radius and height.
     */
    class Cone : public IPrimitive, public Core::ITransformable {
        private:
            Math::Point3D _apex;         // Top point of the cone
            Math::Vector3D _axis;        // Direction from apex to base center
            double _radius;              // Radius of the base circle
            double _height;              // Distance from apex to base
            Color _color;                // Color of the cone
            Math::Point3D _baseCenter;   // Center of the base circle

        public:
            Cone(const Math::Point3D& apex, const Math::Vector3D& axis, double radius, double height);
            Cone(const Math::Point3D& apex, const Math::Vector3D& axis, double radius,
                 double height, const Color& color);
            ~Cone();

            bool hits(const Ray& ray, HitInfo& info) const override;
            const Color& getColor() const override;

            const Math::Point3D& getApex() const;
            const Math::Vector3D& getAxis() const;
            double getRadius() const;
            double getHeight() const;

            const Math::Point3D& getBaseCenter() const;
            void translate(const Math::Vector3D& offset) override;
    };
}
