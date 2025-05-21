#pragma once
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"

namespace RayTracer {
    /**
     * @brief Represents a ray in 3D space with an origin point and a direction
     */
    class Ray {
        public:
            Math::Point3D _origin;     ///< Origin point of the ray
            Math::Vector3D _direction; ///< Normalized direction vector of the ray

            /**
             * @brief Creates a ray from an origin point and a direction
             * @param o Origin point of the ray
             * @param d Direction vector (will be normalized)
             */
            Ray(const Math::Point3D& o, const Math::Vector3D& d);

            /**
             * @brief Destructor
             */
            ~Ray();
    };
}
