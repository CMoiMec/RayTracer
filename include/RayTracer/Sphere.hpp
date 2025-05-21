/*
** Sphere - A simple spherical primitive defined by center and radius
*/
#pragma once
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"
#include "Ray.hpp"
#include "IPrimitive.hpp"
#include "Utils/Color.hpp"
#include "Core/ITransformable.hpp"

namespace RayTracer {
    /**
     * @brief A simple spherical primitive defined by center and radius
     */
    class Sphere : public IPrimitive, public Core::ITransformable {
        private:
            Math::Point3D _center;  // Center point of the sphere
            double _radius;         // Radius of the sphere
            Color _color;           // Color of the sphere

        public:
            Sphere(const Math::Point3D &c, double r);
            Sphere(const Math::Point3D &c, double r, const Color &color);
            ~Sphere();

            bool hits(const Ray&, HitInfo &info) const override;

            const Math::Point3D& getCenter() const;
            double getRadius() const;
            const Color& getColor() const override;

            void translate(const Math::Vector3D& offset) override;
    };
}
