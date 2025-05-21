/*
** Camera - Defines a viewpoint and projection for the scene
**
*/
#pragma once
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"
#include "Math/Rectangle3D.hpp"
#include "Ray.hpp"
#include "Core/ITransformable.hpp"

namespace RayTracer {
    /**
     * @brief Defines a viewpoint and projection for the scene
     */
    class Camera : public Core::ITransformable {
        public:
            Math::Point3D _origin;        // Position of the camera in 3D space
            Math::Vector3D _rotation;     // Rotation angles (in degrees) for the camera
            Math::Rectangle3D _screen;    // Virtual screen through which rays are cast
            double _width;                // Width of the rendered image in pixels
            double _height;               // Height of the rendered image in pixels

            /**
             * @brief Creates a default camera at the origin looking along negative Z axis
             */
            Camera();

            /**
             * @brief Destructor
             */
            ~Camera();

            /**
             * @brief Generates a ray from the camera through a point on the screen
             * @param u Horizontal coordinate (0 to 1) on the screen
             * @param v Vertical coordinate (0 to 1) on the screen
             * @return Ray from camera origin through the specified screen point
             */
            Ray ray(double u, double v) const;

            /**
             * @brief Moves the camera by the given offset vector
             * @param offset Direction and distance to move the camera
             */
            void translate(const Math::Vector3D& offset) override;
    };
}
