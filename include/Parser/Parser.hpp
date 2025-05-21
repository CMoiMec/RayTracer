/*
** Parser - Handles reading and parsing scene configuration files
**
** This class loads scene description files (.cfg) and parses them
** into structured data that can be used to build the scene.
** It supports cameras, primitives, lights and object files.
*/
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <libconfig.h++>

namespace Parser {
    /**
     * @brief 3D vector representation for positions and directions
     */
    struct Vector3D {
        double x;
        double y;
        double z;
    };

    /**
     * @brief Color representation in RGB format
     */
    struct Color {
        int r;
        int g;
        int b;
    };

    /**
     * @brief Image resolution specification
     */
    struct Resolution {
        int width;
        int height;
    };

    /**
     * @brief Camera definition with position, orientation and projection properties
     */
    struct Camera {
        std::string name;
        Resolution resolution;
        Vector3D position;
        Vector3D rotation;
        double fieldOfView;
    };

    /**
     * @brief Sphere primitive definition
     */
    struct Sphere {
        Vector3D position;
        double radius;
        Color color;
    };

    /**
     * @brief Plane primitive definition
     */
    struct Plane {
        std::string axis;
        double position;
        Color color;
    };

    /**
     * @brief Cone primitive definition
     */
    struct Cone {
        Vector3D apex;        // Apex (top) position
        Vector3D axis;        // Direction of the cone axis
        double radius;        // Radius of the base
        double height;        // Height from apex to base
        Color color;
    };

    /**
     * @brief Cylinder primitive definition
     */
    struct Cylinder {
        Vector3D baseCenter;  // Center of the base
        Vector3D axis;        // Direction from base to top
        double radius;        // Radius of the cylinder
        double height;        // Height of the cylinder
        Color color;
    };

    /**
     * @brief Triangle primitive definition
     */
    struct Triangle {
        Vector3D a;     // Vertex a of the triangle
        Vector3D b;     // Vertex b of the triangle
        Vector3D c;     // Vertex c of the triangle
        Color color;
    };

    /**
     * @brief Rectangle primitive definition
     */
    struct Rectangle {
        Vector3D origin;    // Origine of the Rectangle
        Vector3D bottom;    // Bottom Vector
        Vector3D left;      // Left Vector
        Color color;
    };

    /**
     * @brief 3D model file definition
     */
    struct ObjFile {
        std::string path;
        Vector3D position;
        double scale;
        Color color;
    };

    /**
     * @brief Light sources definition
     */
    struct Lights {
        double ambient;
        double diffuse;
        std::vector<Vector3D> point;
        std::vector<Vector3D> directional;
    };

    /**
     * @brief Handles reading and parsing scene configuration files
     */
    class Parser {
    public:
        Parser();
        ~Parser();

        /**
         * @brief Loads and parses a scene configuration file
         * @param filename Path to the scene config file
         * @return True if parsing was successful, false otherwise
         */
        bool loadFromFile(const std::string& filename);

        /**
         * @brief Returns the list of parsed cameras
         * @return Vector of camera definitions
         */
        const std::vector<Camera>& getCameras() const;

        /**
         * @brief Returns the list of parsed spheres
         * @return Vector of sphere definitions
         */
        const std::vector<Sphere>& getSpheres() const;

        /**
         * @brief Returns the list of parsed planes
         * @return Vector of plane definitions
         */
        const std::vector<Plane>& getPlanes() const;

        /**
         * @brief Returns the list of parsed cones
         * @return Vector of cone definitions
         */
        const std::vector<Cone>& getCones() const;

        /**
         * @brief Returns the list of parsed cylinders
         * @return Vector of cylinder definitions
         */
        const std::vector<Cylinder>& getCylinders() const;

        /**
         * @brief Returns the list of parsed triangles
         * @return Vector of cylinder definitions
         */
        const std::vector<Triangle>& getTriangles() const;

        /**
         * @brief Returns the list of parsed rectangles
         * @return Vector of rectangle definitions
        */
        const std::vector<Rectangle>& getRectangles() const;

        /**
         * @brief Returns the list of parsed OBJ files
         * @return Vector of OBJ file definitions
         */
        const std::vector<ObjFile>& getObjFiles() const;

        /**
         * @brief Returns the parsed light sources
         * @return Lights structure with all light definitions
         */
        const Lights& getLights() const;

        /**
         * @brief Checks if parsing encountered any errors
         * @return True if there were errors, false otherwise
         */
        bool hasError() const;

        /**
         * @brief Returns the error message if any
         * @return String containing the error message
         */
        std::string getErrorMessage() const;

    private:
        std::vector<Camera> m_cameras;
        std::vector<Sphere> m_spheres;
        std::vector<Plane> m_planes;
        std::vector<Cone> m_cones;
        std::vector<Triangle> m_triangles;
        std::vector<Rectangle> m_rectangles;
        std::vector<Cylinder> m_cylinders;
        std::vector<ObjFile> m_objFiles;
        Lights m_lights;
        // Error handling
        bool m_hasError;
        std::string m_errorMessage;
    };
}
