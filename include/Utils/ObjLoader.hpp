/*
** ObjLoader - Utility to load and parse .obj 3D model files
**
** Parses OBJ files (Wavefront) and converts them to triangles that
** can be added to the scene. Handles the most common OBJ features.
*/
#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "RayTracer/CompositePrimitive.hpp"
#include "Utils/Color.hpp"

namespace Utils {
    class ObjLoader {
    public:
        /**
         * @brief Loads an OBJ file and converts it to a composite primitive
         *
         * @param objPath Path to the OBJ file
         * @param scale Scale factor to apply to the model
         * @param position Position offset to apply to the model
         * @param color Color to apply to the model
         * @return std::shared_ptr<RayTracer::CompositePrimitive> Composite containing all triangles
         */
        static std::shared_ptr<RayTracer::CompositePrimitive> load(
            const std::string& objPath,
            double scale = 1.0,
            const Math::Point3D& position = Math::Point3D(0, 0, 0),
            const Color& color = Color(255, 255, 255)
        );

    private:
        /**
         * @brief Parse vertex data from an OBJ file
         *
         * @param line Line from OBJ file starting with 'v'
         * @param vertices Output vector of vertices
         * @param scale Scale factor to apply to the model
         * @param position Position offset to apply to the model
         */
        static void parseVertex(
            const std::string& line,
            std::vector<Math::Point3D>& vertices,
            double scale,
            const Math::Point3D& position
        );

        /**
         * @brief Parse face data from an OBJ file
         *
         * @param line Line from OBJ file starting with 'f'
         * @param vertices Vector of parsed vertices
         * @param composite Composite to add triangle faces to
         * @param color Color to apply to faces
         */
        static void parseFace(
            const std::string& line,
            const std::vector<Math::Point3D>& vertices,
            std::shared_ptr<RayTracer::CompositePrimitive>& composite,
            const Color& color
        );
    };
}
