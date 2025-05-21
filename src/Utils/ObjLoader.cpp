/*
** ObjLoader - Implementation of the OBJ file parser
*/
#include "Utils/ObjLoader.hpp"
#include "RayTracer/Triangle.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Utils {

std::shared_ptr<RayTracer::CompositePrimitive> ObjLoader::load(
    const std::string& objPath,
    double scale,
    const Math::Point3D& position,
    const Color& color)
{
    std::ifstream file(objPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open OBJ file: " << objPath << std::endl;
        return std::make_shared<RayTracer::CompositePrimitive>(color);
    }

    std::vector<Math::Point3D> vertices;
    auto composite = std::make_shared<RayTracer::CompositePrimitive>(color);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        if (line[0] == 'v' && (line[1] == ' ' || line[1] == '\t')) {
            parseVertex(line, vertices, scale, position);
        } else if (line[0] == 'f' && (line[1] == ' ' || line[1] == '\t')) {
            parseFace(line, vertices, composite, color);
        }
        // Ignore other OBJ elements like texture coords, normals, etc. for now
    }

    std::cout << "Loaded " << objPath << ": "
              << vertices.size() << " vertices, "
              << composite->getChildCount() << " faces" << std::endl;

    return composite;
}

void ObjLoader::parseVertex(
    const std::string& line,
    std::vector<Math::Point3D>& vertices,
    double scale,
    const Math::Point3D& position)
{
    std::istringstream iss(line.substr(1)); // Skip the 'v' prefix
    double x, y, z;

    if (iss >> x >> y >> z) {
        // Apply scale and position offset
        vertices.push_back(Math::Point3D(
            x * scale + position._x,
            y * scale + position._y,
            z * scale + position._z
        ));
    }
}

void ObjLoader::parseFace(
    const std::string& line,
    const std::vector<Math::Point3D>& vertices,
    std::shared_ptr<RayTracer::CompositePrimitive>& composite,
    const Color& color)
{
    std::istringstream iss(line.substr(1)); // Skip the 'f' prefix
    std::string segment;
    std::vector<int> vertexIndices;

    // Parse vertex indices, handling different OBJ formats:
    // f v1 v2 v3 ...                (simple vertex indices)
    // f v1/vt1/vn1 v2/vt2/vn2 ...   (vertex/texture/normal)
    // f v1//vn1 v2//vn2 ...         (vertex//normal)
    while (iss >> segment) {
        std::istringstream indexStream(segment);
        std::string indexStr;

        if (std::getline(indexStream, indexStr, '/')) {
            try {
                int idx = std::stoi(indexStr);
                // OBJ indices are 1-based, convert to 0-based
                vertexIndices.push_back(idx - 1);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing face index: " << indexStr << std::endl;
            }
        }
    }

    // Create triangles from the face (triangulation for polygons with > 3 vertices)
    if (vertexIndices.size() >= 3) {
        // Convert polygon to triangle fan (simple triangulation for convex polygons)
        for (size_t i = 2; i < vertexIndices.size(); ++i) {
            int idx1 = vertexIndices[0];
            int idx2 = vertexIndices[i-1];
            int idx3 = vertexIndices[i];

            // Ensure indices are valid
            if (idx1 >= 0 && idx1 < static_cast<int>(vertices.size()) &&
                idx2 >= 0 && idx2 < static_cast<int>(vertices.size()) &&
                idx3 >= 0 && idx3 < static_cast<int>(vertices.size())) {

                auto triangle = std::make_shared<RayTracer::Triangle>(
                    vertices[idx1], vertices[idx2], vertices[idx3], color);
                composite->addChild(triangle);
            }
        }
    }
}

}  // namespace Utils
