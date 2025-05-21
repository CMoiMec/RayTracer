#pragma once
#include "RayTracer/ILight.hpp"
#include "Math/Vector3D.hpp"

namespace RayTracer {
    /**
     * @brief Light source with parallel rays in a specific direction
     */
    class DirectionalLight : public ILight {
        private:
            Math::Vector3D m_direction;  // Direction the light is coming from
            double m_intensity;          // Intensity factor of the light
            Color _color;                // Color of the light

        public:
            /**
             * @brief Creates a directional light with specified direction, color and intensity
             * @param direction The direction the light is coming from (will be normalized)
             * @param col The light color (defaults to white)
             * @param intensity The light intensity (defaults to 1.0)
             */
            DirectionalLight(const Math::Vector3D& direction, const Color& col = Color(255,255,255), double intensity = 1.0);

            virtual ~DirectionalLight() = default;

            /**
             * @brief Gets the light direction
             * @return Normalized direction vector the light is coming from
             */
            const Math::Vector3D& getDirection() const;

            double getIntensity() const override;
            Color getColor() const override;
    };
}
