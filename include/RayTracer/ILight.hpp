/*
** ILight - Interface for all light sources in the scene
*/
#pragma once
#include "Utils/Color.hpp"
#include "Math/Point3D.hpp"

namespace RayTracer {
    /**
     * @brief Interface for all light sources in the scene
     */
    class ILight {
        public:
            /**
             * @brief Virtual destructor
             */
            virtual ~ILight() = default;

            /**
             * @brief Gets the color of the light
             * @return The RGB color of the light
             */
            virtual Color getColor() const = 0;

            /**
             * @brief Gets the intensity of the light
             * @return Intensity factor (typically 0.0 to 1.0)
             */
            virtual double getIntensity() const = 0;
    };
}
