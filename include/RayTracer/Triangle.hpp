/*
** Triangle - A three-vertex planar primitive in 3D space
*/
#pragma once
#include "Math/Point3D.hpp"
#include "Ray.hpp"
#include "IPrimitive.hpp"
#include "Core/ITransformable.hpp"

namespace RayTracer {
    /**
     * @brief A renderable triangle primitive defined by 3 points.
     * The points can be modified directly if needed.
     */
    class Triangle : public IPrimitive, public Core::ITransformable {
        public:
            Triangle();

            Triangle(const Math::Point3D& a, const Math::Point3D& b, const Math::Point3D& c,
            const Color& col = Color(255,255,255));
            ~Triangle();

            Math::Point3D _a;
            Math::Point3D _b;
            Math::Point3D _c;
            // Color of the triangle
            Color _color;

            /**
            * @brief Checks if a ray intersects this triangle.
            * @param ray is ray
            * @param info Output parameter that will contain hit information if the ray intersects
            * @return true if the ray intersects the triangle, false otherwise
            */
            bool hits(const Ray& ray, HitInfo& info) const override;

            // Moves the triangle by the given offset vector
            void translate(const Math::Vector3D& offset) override;
            const Color& getColor() const;
    };
}
