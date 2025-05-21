#pragma once
#include "Ray.hpp"
#include "Utils/Color.hpp"
#include "RayTracer/HitInfo.hpp"

namespace RayTracer {
    /**
     * @brief Base interface for all geometric objects in the scene
     */
    class IPrimitive {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IPrimitive() = default;

            /**
             * @brief Tests if a ray intersects this primitive
             * @param ray The ray to test for intersection
             * @param info Output parameter filled with hit information if intersection occurs
             * @return True if the ray hits the primitive, false otherwise
             */
            virtual bool hits(const Ray& ray, HitInfo& info) const = 0;

            /**
             * @brief Gets the color of this primitive
             * @return Reference to the primitive's color
             */
            virtual const Color& getColor() const = 0;
    };
}
