/*
** Scene - Implementation of the scene loading and management
*/

#include "Core/Scene.hpp"
#include "Parser/Parser.hpp"
#include "RayTracer/CompositePrimitive.hpp"
#include "RayTracer/AmbientLight.hpp"
#include "RayTracer/DirectionalLight.hpp"
#include "Utils/Color.hpp"
#include "Core/PrimitiveConfig.hpp"
#include "RayTracer/PointLight.hpp"
#include "Utils/ObjLoader.hpp"
#include <iostream>
#include <cmath>

Scene::Scene(Core::PrimitiveFactory &fac) : _factory(fac)
{}

Scene::~Scene() {}

double deg2rad (double d)
{
    return d*M_PI/180.0;
}

void Scene::loadFromFile(const std::string& path) {
    Parser::Parser parser;

    if (!parser.loadFromFile(path)) {
        throw std::runtime_error(parser.getErrorMessage());
    }

    const auto& parsedCameras = parser.getCameras();
    for (const auto& parsedCam : parsedCameras) {
        auto camera = std::make_shared<RayTracer::Camera>();

        double aspect = double(parsedCam.resolution.width)
                    / parsedCam.resolution.height;
        double fovRad = parsedCam.fieldOfView * M_PI/180.0;
        double screenH = 2.0 * std::tan(fovRad/2.0);
        double screenW = screenH * aspect;

        // 1) Calcul du forward à partir des angles (pitch=X, yaw=Y, roll=Z)
        double pitch = deg2rad(parsedCam.rotation.x);
        double yaw   = deg2rad(parsedCam.rotation.y);
        double roll  = deg2rad(parsedCam.rotation.z);

        // matrice de rotation ZYX ou applique successivement
        Math::Vector3D forward(0, 0, -1);
        forward = forward.rotateX(pitch)
                        .rotateY(yaw)
                        .rotateZ(roll);

        // 2) Définir up, right
        Math::Vector3D worldUp(0,1,0);
        Math::Vector3D right = forward.cross(worldUp).normalize();
        Math::Vector3D up    = right.cross(forward).normalize();

        // 3) Calculer centre et côtés de l'écran
        // distance fixe de 1 unité devant la caméra (ou toute autre)
        double d = 1.0;
        Math::Point3D  camPos(parsedCam.position.x,
                            parsedCam.position.y,
                            parsedCam.position.z);
        Math::Point3D  screenCenter = camPos + forward * d;
        Math::Vector3D bottomSide   = right * screenW;
        Math::Vector3D leftSide     = up    * screenH;
        // coin bas-gauche de l'écran :
        Math::Point3D  origin3D =  screenCenter
                                - bottomSide * 0.5
                                - leftSide   * 0.5;

        // 4) Affecter à la caméra
        camera->_origin = camPos;
        camera->_rotation = Math::Vector3D(
            parsedCam.rotation.x,
            parsedCam.rotation.y,
            parsedCam.rotation.z
        );
        camera->_width  = parsedCam.resolution.width;
        camera->_height = parsedCam.resolution.height;
        camera->_screen = Math::Rectangle3D(origin3D, bottomSide, leftSide);

        cameras.push_back(camera);
        cameraMap[parsedCam.name] = camera;
    }

    const auto& parsedSpheres = parser.getSpheres();
    for (const auto& ps : parsedSpheres) {
        PrimitiveConfig cfg;
        cfg.type  = "sphere";
        cfg.color = Color(ps.color.r, ps.color.g, ps.color.b);
        cfg.data.emplace<SphereData_t>(SphereData_t{
        Math::Point3D{ps.position.x, ps.position.y, ps.position.z},
        ps.radius});
        auto sphere = _factory.create(cfg.type, cfg);
        primitives.push_back(std::move(sphere));
    }

    const auto& parsedPlanes = parser.getPlanes();
    for (const auto& pp : parsedPlanes) {
        PrimitiveConfig cfg;
        cfg.type  = "plane";
        cfg.color = Color(pp.color.r, pp.color.g, pp.color.b);

        PlaneData_t pd;
        if (pp.axis == "X") {
            pd.norm   = Math::Vector3D(1, 0, 0);
            pd.pos = Math::Point3D(pp.position, 0, 0);
        }
        else if (pp.axis == "Y") {
            pd.norm   = Math::Vector3D(0, 1, 0);
            pd.pos = Math::Point3D(0, pp.position, 0);
        }
        else {
            pd.norm   = Math::Vector3D(0, 0, 1);
            pd.pos= Math::Point3D(0, 0, pp.position);
        }
        cfg.data.emplace<PlaneData_t>(pd);
        auto plane = _factory.create(cfg.type, cfg);
        primitives.push_back(std::move(plane));
    }

    const auto& parsedCones = parser.getCones();
    for (const auto& pc : parsedCones) {
        PrimitiveConfig cfg;
        cfg.type  = "cone";
        cfg.color = Color(pc.color.r, pc.color.g, pc.color.b);

        cfg.data.emplace<ConeData_t>(ConeData_t{
            Math::Point3D{ pc.apex.x, pc.apex.y, pc.apex.z },
            Math::Vector3D{ pc.axis.x, pc.axis.y, pc.axis.z },
            pc.radius,
            pc.height
        });
        auto cone = _factory.create(cfg.type, cfg);
        primitives.push_back(std::move(cone));
    }

    const auto& parsedCylinders = parser.getCylinders();
    for (const auto& cyl : parsedCylinders) {
        PrimitiveConfig cfg;
        cfg.type  = "cylinder";
        cfg.color = Color(cyl.color.r, cyl.color.g, cyl.color.b);

        cfg.data.emplace<CylinderData_t>(CylinderData_t{
            Math::Point3D{ cyl.baseCenter.x, cyl.baseCenter.y, cyl.baseCenter.z },
            Math::Vector3D{ cyl.axis.x, cyl.axis.y, cyl.axis.z },
            cyl.radius,
            cyl.height
        });
        auto cylinder = _factory.create(cfg.type, cfg);
        primitives.push_back(std::move(cylinder));
    }

    std::cout << "les triangles c'est ici" << std::endl;
    const auto& parsedTriangles = parser.getTriangles();
    for (const auto& pt : parsedTriangles) {
        PrimitiveConfig cfg;
        cfg.type  = "triangle";
        cfg.color = Color(pt.color.r, pt.color.g, pt.color.b);

        cfg.data.emplace<TriangleData_t>(TriangleData_t{
            // sommets a, b, c
            Math::Point3D{ pt.a.x, pt.a.y, pt.a.z },
            Math::Point3D{ pt.b.x, pt.b.y, pt.b.z },
            Math::Point3D{ pt.c.x, pt.c.y, pt.c.z }
        });

        auto triangle = _factory.create(cfg.type, cfg);
        primitives.push_back(std::move(triangle));
        std::cout << "triangles build" << std::endl;
    }

    const auto& parsedRectangles = parser.getRectangles();
    for (const auto& pr : parsedRectangles) {
        PrimitiveConfig cfg;
        cfg.type  = "rectangle";
        cfg.color = Color(pr.color.r, pr.color.g, pr.color.b);

        cfg.data.emplace<RectangleData_t>(RectangleData_t{
            // origine
            Math::Point3D{ pr.origin.x, pr.origin.y, pr.origin.z },
            // bottom
            Math::Vector3D{ pr.bottom.x, pr.bottom.y, pr.bottom.z },
            // left
            Math::Vector3D{ pr.left.x,   pr.left.y,   pr.left.z }
        });

        auto rect = _factory.create(cfg.type, cfg);
        primitives.push_back(std::move(rect));
    }

    const auto& parsedObjFiles = parser.getObjFiles();
    for (const auto& parsedObj : parsedObjFiles) {
        Color objColor(parsedObj.color.r, parsedObj.color.g, parsedObj.color.b);
        Math::Point3D objPosition(parsedObj.position.x, parsedObj.position.y, parsedObj.position.z);

        auto objModel = Utils::ObjLoader::load(
            parsedObj.path,
            parsedObj.scale,
            objPosition,
            objColor
        );

        primitives.push_back(objModel);
    }

    const auto& parsedLights = parser.getLights();

    if (parsedLights.ambient > 0) {
        auto ambientLight = std::make_shared<RayTracer::AmbientLight>(
            parsedLights.ambient
        );
        lights.push_back(ambientLight);
    }

    for (const auto& dir : parsedLights.directional) {
        auto dirLight = std::make_shared<RayTracer::DirectionalLight>(
            Math::Vector3D(dir.x, dir.y, dir.z)
            //,
            //parsedLights.diffuse
        );
        lights.push_back(dirLight);
    }

    for (const auto& point : parsedLights.point) {
        auto pointLight = std::make_shared<RayTracer::PointLight>(
            Math::Point3D(point.x, point.y, point.z),
            Color(255,255,255), 1.0
        );
        lights.push_back(pointLight);
    }

    std::cout << "Scene loaded successfully:" << std::endl;
    std::cout << "  - " << cameras.size() << " cameras" << std::endl;
    std::cout << "  - " << primitives.size() << " primitives" << std::endl;

    int sphereCount = parser.getSpheres().size();
    int planeCount = parser.getPlanes().size();
    int coneCount = parser.getCones().size();
    int cylinderCount = parser.getCylinders().size();
    int objCount = parser.getObjFiles().size();

    std::cout << " (" << sphereCount << " spheres, "
              << planeCount << " planes, "
              << coneCount << " cones, "
              << cylinderCount << " cylinders, "
              << objCount << " obj models)" << std::endl;

    std::cout << "  - " << lights.size() << " lights" << std::endl;
}

std::shared_ptr<RayTracer::Camera> Scene::getCameraByName(const std::string& name) const {
    auto it = cameraMap.find(name);
    if (it != cameraMap.end()) {
        return it->second;
    }

    return nullptr;
}

bool Scene::moveObject(const std::string& name, const Math::Vector3D& offset) {
    auto it = cameraMap.find(name);
    if (it == cameraMap.end())
        return false;

    it->second->translate(offset);
    return true;
}
