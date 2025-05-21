/*
** PointLight - Light source that radiates in all directions from a single point
*/
#pragma once
#include "RayTracer/ILight.hpp"
#include "Math/Point3D.hpp"
#include "Core/ITransformable.hpp"

namespace RayTracer {
    /**
     * @brief Light source that radiates in all directions from a single point
     */
    class PointLight : public ILight, public Core::ITransformable {
    private:
        Math::Point3D m_position;    // Position of the light in 3D space
        double m_intensity;          // Base intensity of the light
        Color m_color;               // Color of the light

    public:
        /**
         * @brief Creates a point light at the specified position
         * @param pos Position of the light in 3D space
         * @param col Color of the light (defaults to white)
         * @param intensity Base intensity of the light (defaults to 1.0)
         */
        PointLight(const Math::Point3D& pos,
               const Color& col = Color(255,255,255),
               double intensity = 1.0);

        /**
         * @brief Destructor
         */
        ~PointLight() = default;

        /**
         * @brief Gets the position of the light
         * @return 3D position of the light
         */
        const Math::Point3D& getPosition() const;

        /**
         * @brief Gets the base intensity of the light (before distance attenuation)
         * @return Intensity value
         */
        double getIntensity() const override;

        /**
         * @brief Gets the color of the light
         * @return RGB color
         */
        Color getColor() const override;

        /**
         * @brief Moves the light by the specified offset
         * @param offset Direction and distance to move
         */
        void translate(const Math::Vector3D& offset) override;
    };
}
