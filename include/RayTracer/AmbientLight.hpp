/*
**
**
**
**
*/
#pragma once
#include "RayTracer/ILight.hpp"

namespace RayTracer {
    /**
     * @brief Uniform background illumination for the entire scene
     */
    class AmbientLight : public ILight {
    private:
        double m_intensity;    // Intensity factor (0.0 to 1.0)
        Color m_color;         // Color of the ambient light

    public:
        AmbientLight(double intensity, const Color& col = Color(255,255,255));
        ~AmbientLight() = default;

        double getIntensity() const override;
        Color getColor() const override;
    };
}
