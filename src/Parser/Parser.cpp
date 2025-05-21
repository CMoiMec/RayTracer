#include "Parser/Parser.hpp"
#include <iostream>
#include <libconfig.h++>

namespace Parser {

Parser::Parser() : m_hasError(false) {}

Parser::~Parser() {}

template<typename T>
bool safeGetValue(const libconfig::Setting& setting, const char* key, T& value) {
    try {
        if (!setting.exists(key)) {
            std::cerr << "Key '" << key << "' does not exist" << std::endl;
            return false;
        }

        const libconfig::Setting& s = setting[key];

        if (s.isNumber()) {
            if (s.getType() == libconfig::Setting::TypeInt) {
                value = static_cast<T>(static_cast<int>(s));
                return true;
            } else if (s.getType() == libconfig::Setting::TypeInt64) {
                value = static_cast<T>(static_cast<long long>(s));
                return true;
            } else if (s.getType() == libconfig::Setting::TypeFloat) {
                value = static_cast<T>(static_cast<double>(s));
                return true;
            }
        }

        std::cerr << "Key '" << key << "' is not a number or cannot be converted to the requested type" << std::endl;
        std::cerr << "Type is: " << s.getType() << std::endl;
        return false;
    } catch (const libconfig::SettingTypeException& ex) {
        std::cerr << "Type error for key '" << key << "': " << ex.what() << std::endl;
        return false;
    } catch (const std::exception& ex) {
        std::cerr << "Error for key '" << key << "': " << ex.what() << std::endl;
        return false;
    }
}

bool safeGetString(const libconfig::Setting& setting, const char* key, std::string& value) {
    try {
        if (!setting.exists(key)) {
            std::cerr << "Key '" << key << "' does not exist" << std::endl;
            return false;
        }

        const libconfig::Setting& s = setting[key];

        if (s.getType() == libconfig::Setting::TypeString) {
            value = static_cast<const char*>(s);
            return true;
        }

        std::cerr << "Key '" << key << "' is not a string" << std::endl;
        return false;
    } catch (const libconfig::SettingTypeException& ex) {
        std::cerr << "Type error for key '" << key << "': " << ex.what() << std::endl;
        return false;
    } catch (const std::exception& ex) {
        std::cerr << "Error for key '" << key << "': " << ex.what() << std::endl;
        return false;
    }
}

bool Parser::loadFromFile(const std::string& filename)
{
    libconfig::Config config;

    try {
        config.readFile(filename.c_str());
    } catch(const libconfig::FileIOException& ex) {
        m_hasError = true;
        m_errorMessage = "Error: Unable to read file " + filename;
        return false;
    } catch(const libconfig::ParseException& ex) {
        m_hasError = true;
        m_errorMessage = "Parse error at " + std::string(ex.getFile()) + ":" +
                         std::to_string(ex.getLine()) + " - " + ex.getError();
        return false;
    }

    try {
        // Parse cameras
        if (config.exists("cameras")) {
            const libconfig::Setting& cameras = config.lookup("cameras");

            // Debug:
            std::cout << "Type de 'cameras': " << cameras.getType() << std::endl;
            std::cout << "Nombre de caméras: " << cameras.getLength() << std::endl;

            for (int i = 0; i < cameras.getLength(); ++i) {
                Camera camera;
                // Default value
                camera.position = {0.0, 0.0, 0.0};
                camera.rotation = {0.0, 0.0, 0.0};
                camera.resolution = {800, 600};
                camera.fieldOfView = 60.0;

                const libconfig::Setting& cam = cameras[i];

                // Debug:
                std::cout << "Camera " << i << " type: " << cam.getType() << std::endl;

                // Name
                std::string name;
                if (safeGetString(cam, "name", name)) {
                    camera.name = name;
                } else {
                    camera.name = "camera_" + std::to_string(i);
                }

                // Resolution
                if (cam.exists("resolution")) {
                    const libconfig::Setting& res = cam["resolution"];
                    int width, height;
                    if (safeGetValue(res, "width", width)) {
                        camera.resolution.width = width;
                    }
                    if (safeGetValue(res, "height", height)) {
                        camera.resolution.height = height;
                    }
                }

                // Position
                if (cam.exists("position")) {
                    const libconfig::Setting& pos = cam["position"];

                    // Debug:
                    std::cout << "Position type: " << pos.getType() << std::endl;
                    for (int j = 0; j < pos.getLength(); ++j) {
                        if (pos[j].getName()) {
                            std::cout << "  " << pos[j].getName() << " type: " << pos[j].getType() << std::endl;
                        }
                    }

                    double x, y, z;
                    if (safeGetValue(pos, "x", x)) {
                        camera.position.x = x;
                    }
                    if (safeGetValue(pos, "y", y)) {
                        camera.position.y = y;
                    }
                    if (safeGetValue(pos, "z", z)) {
                        camera.position.z = z;
                    }
                }

                // Rotation
                if (cam.exists("rotation")) {
                    const libconfig::Setting& rot = cam["rotation"];
                    double x, y, z;
                    if (safeGetValue(rot, "x", x)) {
                        camera.rotation.x = x;
                    }
                    if (safeGetValue(rot, "y", y)) {
                        camera.rotation.y = y;
                    }
                    if (safeGetValue(rot, "z", z)) {
                        camera.rotation.z = z;
                    }
                }

                // FOV
                double fov;
                if (safeGetValue(cam, "fieldOfView", fov)) {
                    camera.fieldOfView = fov;
                }

                // Debug:
                std::cout << "Camera " << i << " values:" << std::endl;
                std::cout << "  Name: " << camera.name << std::endl;
                std::cout << "  Resolution: " << camera.resolution.width << "x" << camera.resolution.height << std::endl;
                std::cout << "  Position: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")" << std::endl;
                std::cout << "  Rotation: (" << camera.rotation.x << ", " << camera.rotation.y << ", " << camera.rotation.z << ")" << std::endl;
                std::cout << "  FOV: " << camera.fieldOfView << std::endl;

                m_cameras.push_back(camera);
            }
        }

        // Parse primitives
        if (config.exists("primitives")) {
            const libconfig::Setting& primitives = config.lookup("primitives");

            // Parse spheres
            if (primitives.exists("spheres")) {
                const libconfig::Setting& spheres = primitives["spheres"];

                for (int i = 0; i < spheres.getLength(); ++i) {
                    Sphere sphere;
                    // Default value
                    sphere.position = {0.0, 0.0, 0.0};
                    sphere.radius = 1.0;
                    sphere.color = {255, 255, 255};

                    const libconfig::Setting& sph = spheres[i];

                    // Position and radius
                    double x, y, z, r;
                    if (safeGetValue(sph, "x", x)) {
                        sphere.position.x = x;
                    }
                    if (safeGetValue(sph, "y", y)) {
                        sphere.position.y = y;
                    }
                    if (safeGetValue(sph, "z", z)) {
                        sphere.position.z = z;
                    }
                    if (safeGetValue(sph, "r", r)) {
                        sphere.radius = r;
                    }

                    // Color
                    if (sph.exists("color")) {
                        const libconfig::Setting& col = sph["color"];
                        int r, g, b;
                        if (safeGetValue(col, "r", r)) {
                            sphere.color.r = r;
                        }
                        if (safeGetValue(col, "g", g)) {
                            sphere.color.g = g;
                        }
                        if (safeGetValue(col, "b", b)) {
                            sphere.color.b = b;
                        }
                    }

                    m_spheres.push_back(sphere);
                }
            }

            // Parse planes
            if (primitives.exists("planes")) {
                const libconfig::Setting& planes = primitives["planes"];

                for (int i = 0; i < planes.getLength(); ++i) {
                    Plane plane;
                    // Default value
                    plane.axis = "Z";
                    plane.position = 0.0;
                    plane.color = {255, 255, 255};

                    const libconfig::Setting& pln = planes[i];

                    // Axis
                    std::string axis;
                    if (safeGetString(pln, "axis", axis)) {
                        plane.axis = axis;
                    }

                    // Position
                    double position;
                    if (safeGetValue(pln, "position", position)) {
                        plane.position = position;
                    }

                    // Color
                    if (pln.exists("color")) {
                        const libconfig::Setting& col = pln["color"];
                        int r, g, b;
                        if (safeGetValue(col, "r", r)) {
                            plane.color.r = r;
                        }
                        if (safeGetValue(col, "g", g)) {
                            plane.color.g = g;
                        }
                        if (safeGetValue(col, "b", b)) {
                            plane.color.b = b;
                        }
                    }

                    m_planes.push_back(plane);
                }
            }

            // Parse cones
            if (primitives.exists("cones")) {
                const libconfig::Setting& cones = primitives["cones"];

                for (int i = 0; i < cones.getLength(); ++i) {
                    Cone cone;
                    // Default values
                    cone.apex = {0.0, 0.0, 0.0};
                    cone.axis = {0.0, 1.0, 0.0};  // Default points up
                    cone.radius = 1.0;
                    cone.height = 1.0;
                    cone.color = {255, 255, 255};

                    const libconfig::Setting& con = cones[i];

                    // Apex position
                    if (con.exists("apex")) {
                        const libconfig::Setting& apex = con["apex"];
                        double x, y, z;
                        if (safeGetValue(apex, "x", x)) {
                            cone.apex.x = x;
                        }
                        if (safeGetValue(apex, "y", y)) {
                            cone.apex.y = y;
                        }
                        if (safeGetValue(apex, "z", z)) {
                            cone.apex.z = z;
                        }
                    }

                    // Axis direction
                    if (con.exists("axis")) {
                        const libconfig::Setting& axis = con["axis"];
                        double x, y, z;
                        if (safeGetValue(axis, "x", x)) {
                            cone.axis.x = x;
                        }
                        if (safeGetValue(axis, "y", y)) {
                            cone.axis.y = y;
                        }
                        if (safeGetValue(axis, "z", z)) {
                            cone.axis.z = z;
                        }
                    }

                    // Radius and height
                    double radius, height;
                    if (safeGetValue(con, "radius", radius)) {
                        cone.radius = radius;
                    }
                    if (safeGetValue(con, "height", height)) {
                        cone.height = height;
                    }

                    // Color
                    if (con.exists("color")) {
                        const libconfig::Setting& col = con["color"];
                        int r, g, b;
                        if (safeGetValue(col, "r", r)) {
                            cone.color.r = r;
                        }
                        if (safeGetValue(col, "g", g)) {
                            cone.color.g = g;
                        }
                        if (safeGetValue(col, "b", b)) {
                            cone.color.b = b;
                        }
                    }

                    m_cones.push_back(cone);
                }
            }

            // Parse cylinders
            if (primitives.exists("cylinders")) {
                const libconfig::Setting& cylinders = primitives["cylinders"];

                for (int i = 0; i < cylinders.getLength(); ++i) {
                    Cylinder cylinder;
                    // Default values
                    cylinder.baseCenter = {0.0, 0.0, 0.0};
                    cylinder.axis = {0.0, 1.0, 0.0};  // Default points up
                    cylinder.radius = 1.0;
                    cylinder.height = 1.0;
                    cylinder.color = {255, 255, 255};

                    const libconfig::Setting& cyl = cylinders[i];

                    // Base center position
                    if (cyl.exists("baseCenter")) {
                        const libconfig::Setting& base = cyl["baseCenter"];
                        double x, y, z;
                        if (safeGetValue(base, "x", x)) {
                            cylinder.baseCenter.x = x;
                        }
                        if (safeGetValue(base, "y", y)) {
                            cylinder.baseCenter.y = y;
                        }
                        if (safeGetValue(base, "z", z)) {
                            cylinder.baseCenter.z = z;
                        }
                    }

                    // Axis direction
                    if (cyl.exists("axis")) {
                        const libconfig::Setting& axis = cyl["axis"];
                        double x, y, z;
                        if (safeGetValue(axis, "x", x)) {
                            cylinder.axis.x = x;
                        }
                        if (safeGetValue(axis, "y", y)) {
                            cylinder.axis.y = y;
                        }
                        if (safeGetValue(axis, "z", z)) {
                            cylinder.axis.z = z;
                        }
                    }

                    // Radius and height
                    double radius, height;
                    if (safeGetValue(cyl, "radius", radius)) {
                        cylinder.radius = radius;
                    }
                    if (safeGetValue(cyl, "height", height)) {
                        cylinder.height = height;
                    }

                    // Color
                    if (cyl.exists("color")) {
                        const libconfig::Setting& col = cyl["color"];
                        int r, g, b;
                        if (safeGetValue(col, "r", r)) {
                            cylinder.color.r = r;
                        }
                        if (safeGetValue(col, "g", g)) {
                            cylinder.color.g = g;
                        }
                        if (safeGetValue(col, "b", b)) {
                            cylinder.color.b = b;
                        }
                    }

                    m_cylinders.push_back(cylinder);
                }
            }

            //Parse triangles
            if (primitives.exists("triangles")) {
                const libconfig::Setting& triangles = primitives["triangles"];

                for (int i = 0; i < triangles.getLength(); ++i) {
                    Triangle tri;
                    tri.a     = {0.0, 0.0, 0.0};
                    tri.b     = {0.0, 0.0, 0.0};
                    tri.c     = {0.0, 0.0, 0.0};
                    tri.color = {255, 255, 255};

                    const libconfig::Setting& t = triangles[i];

                    if (t.exists("a")) {
                        const libconfig::Setting& A = t["a"];
                        double x, y, z;
                        if (safeGetValue(A, "x", x)) tri.a.x = x;
                        if (safeGetValue(A, "y", y)) tri.a.y = y;
                        if (safeGetValue(A, "z", z)) tri.a.z = z;
                    }

                    if (t.exists("b")) {
                        const libconfig::Setting& B = t["b"];
                        double x, y, z;
                        if (safeGetValue(B, "x", x)) tri.b.x = x;
                        if (safeGetValue(B, "y", y)) tri.b.y = y;
                        if (safeGetValue(B, "z", z)) tri.b.z = z;
                    }

                    if (t.exists("c")) {
                        const libconfig::Setting& C = t["c"];
                        double x, y, z;
                        if (safeGetValue(C, "x", x)) tri.c.x = x;
                        if (safeGetValue(C, "y", y)) tri.c.y = y;
                        if (safeGetValue(C, "z", z)) tri.c.z = z;
                    }

                    if (t.exists("color")) {
                        const libconfig::Setting& col = t["color"];
                        int r, g, b;
                        if (safeGetValue(col, "r", r)) tri.color.r = r;
                        if (safeGetValue(col, "g", g)) tri.color.g = g;
                        if (safeGetValue(col, "b", b)) tri.color.b = b;
                    }
                    m_triangles.push_back(tri);
                }
            }

            // Parse rectangles
            if (primitives.exists("rectangles")) {
                const libconfig::Setting& rects = primitives["rectangles"];

                for (int i = 0; i < rects.getLength(); ++i) {
                    Rectangle rect;
                    rect.origin = {0.0, 0.0, 0.0};
                    rect.bottom = {0.0, 0.0, 0.0};
                    rect.left   = {0.0, 0.0, 0.0};
                    rect.color  = {255, 255, 255};

                    const libconfig::Setting& r = rects[i];

                    if (r.exists("origin")) {
                        const libconfig::Setting& o = r["origin"];
                        double x, y, z;
                        if (safeGetValue(o, "x", x)) rect.origin.x = x;
                        if (safeGetValue(o, "y", y)) rect.origin.y = y;
                        if (safeGetValue(o, "z", z)) rect.origin.z = z;
                    }

                    if (r.exists("bottom")) {
                        const libconfig::Setting& b = r["bottom"];
                        double x, y, z;
                        if (safeGetValue(b, "x", x)) rect.bottom.x = x;
                        if (safeGetValue(b, "y", y)) rect.bottom.y = y;
                        if (safeGetValue(b, "z", z)) rect.bottom.z = z;
                    }

                    if (r.exists("left")) {
                        const libconfig::Setting& l = r["left"];
                        double x, y, z;
                        if (safeGetValue(l, "x", x)) rect.left.x = x;
                        if (safeGetValue(l, "y", y)) rect.left.y = y;
                        if (safeGetValue(l, "z", z)) rect.left.z = z;
                    }

                    if (r.exists("color")) {
                        const libconfig::Setting& c = r["color"];
                        int cr, cg, cb;
                        if (safeGetValue(c, "r", cr)) rect.color.r = cr;
                        if (safeGetValue(c, "g", cg)) rect.color.g = cg;
                        if (safeGetValue(c, "b", cb)) rect.color.b = cb;
                    }
                    m_rectangles.push_back(rect);
                }
            }
            
            // Parse OBJ files
            if (primitives.exists("obj_files")) {
                const libconfig::Setting& objFiles = primitives["obj_files"];

                for (int i = 0; i < objFiles.getLength(); ++i) {
                    ObjFile objFile;
                    // Default value
                    objFile.path = "";
                    objFile.position = {0.0, 0.0, 0.0};
                    objFile.scale = 1.0;
                    objFile.color = {255, 255, 255};

                    const libconfig::Setting& obj = objFiles[i];

                    // Path
                    std::string path;
                    if (safeGetString(obj, "path", path)) {
                        objFile.path = path;
                    }

                    // Position
                    if (obj.exists("position")) {
                        const libconfig::Setting& pos = obj["position"];
                        double x, y, z;
                        if (safeGetValue(pos, "x", x)) {
                            objFile.position.x = x;
                        }
                        if (safeGetValue(pos, "y", y)) {
                            objFile.position.y = y;
                        }
                        if (safeGetValue(pos, "z", z)) {
                            objFile.position.z = z;
                        }
                    }

                    // Scale
                    double scale;
                    if (safeGetValue(obj, "scale", scale)) {
                        objFile.scale = scale;
                    }

                    // Color
                    if (obj.exists("color")) {
                        const libconfig::Setting& col = obj["color"];
                        int r, g, b;
                        if (safeGetValue(col, "r", r)) {
                            objFile.color.r = r;
                        }
                        if (safeGetValue(col, "g", g)) {
                            objFile.color.g = g;
                        }
                        if (safeGetValue(col, "b", b)) {
                            objFile.color.b = b;
                        }
                    }

                    m_objFiles.push_back(objFile);
                }
            }
        }

        // Parse lights
        if (config.exists("lights")) {
            const libconfig::Setting& lights = config.lookup("lights");

            // Default value
            m_lights.ambient = 0.0;
            m_lights.diffuse = 0.0;

            // Ambient and diffuse
            double ambient, diffuse;
            if (safeGetValue(lights, "ambient", ambient)) {
                m_lights.ambient = ambient;
            }
            if (safeGetValue(lights, "diffuse", diffuse)) {
                m_lights.diffuse = diffuse;
            }

            // Parse point lights
            if (lights.exists("point")) {
                const libconfig::Setting& pointLights = lights["point"];

                for (int i = 0; i < pointLights.getLength(); ++i) {
                    Vector3D point = {0.0, 0.0, 0.0};

                    const libconfig::Setting& light = pointLights[i];

                    double x, y, z;
                    if (safeGetValue(light, "x", x)) {
                        point.x = x;
                    }
                    if (safeGetValue(light, "y", y)) {
                        point.y = y;
                    }
                    if (safeGetValue(light, "z", z)) {
                        point.z = z;
                    }

                    m_lights.point.push_back(point);
                }
            }

            // Parse directional lights
            if (lights.exists("directional")) {
                const libconfig::Setting& dirLights = lights["directional"];

                for (int i = 0; i < dirLights.getLength(); ++i) {
                    Vector3D dir = {0.0, 0.0, 0.0};

                    const libconfig::Setting& light = dirLights[i];

                    double x, y, z;
                    if (safeGetValue(light, "x", x)) {
                        dir.x = x;
                    }
                    if (safeGetValue(light, "y", y)) {
                        dir.y = y;
                    }
                    if (safeGetValue(light, "z", z)) {
                        dir.z = z;
                    }

                    m_lights.directional.push_back(dir);
                }
            }
        }

        return true;
    } catch(const libconfig::SettingNotFoundException& ex) {
        m_hasError = true;
        m_errorMessage = "Error: Required setting '" + std::string(ex.getPath()) + "' not found in configuration";
        std::cerr << m_errorMessage << std::endl;
        return false;
    } catch(const libconfig::SettingTypeException& ex) {
        m_hasError = true;
        m_errorMessage = "Error: Invalid type for setting '" + std::string(ex.getPath()) + "'";
        std::cerr << m_errorMessage << std::endl;
        return false;
    } catch(const std::exception& ex) {
        m_hasError = true;
        m_errorMessage = "Error: " + std::string(ex.what());
        std::cerr << m_errorMessage << std::endl;
        return false;
    }
}

const std::vector<Camera>& Parser::getCameras() const
{
    return m_cameras;
}

const std::vector<Sphere>& Parser::getSpheres() const
{
    return m_spheres;
}

const std::vector<Plane>& Parser::getPlanes() const
{
    return m_planes;
}

const std::vector<Cone>& Parser::getCones() const
{
    return m_cones;
}

const std::vector<Cylinder>& Parser::getCylinders() const
{
    return m_cylinders;
}

const std::vector<Triangle>& Parser::getTriangles() const
{
    return m_triangles;
}

const std::vector<Rectangle>& Parser::getRectangles() const
{
    return m_rectangles;
}

const std::vector<ObjFile>& Parser::getObjFiles() const
{
    return m_objFiles;
}

const Lights& Parser::getLights() const
{
    return m_lights;
}

bool Parser::hasError() const
{
    return m_hasError;
}

std::string Parser::getErrorMessage() const
{
    return m_errorMessage;
}

} // namespace Parser
