/*
** Scene - Central container and manager for all scene elements
**
** Manages the complete 3D scene, including cameras, primitives, and lights.
** Responsible for loading scene definitions from configuration files and
** providing access to scene elements during rendering.
*/
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "RayTracer/Camera.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "RayTracer/ILight.hpp"
#include "Core/PrimitiveFactory.hpp"
#include "Core/PrimitiveConfig.hpp"

/**
 * @brief Central container and manager for all scene elements
 */
class Scene {
    public:
        Scene(Core::PrimitiveFactory& fac);
        ~Scene();

        void loadFromFile(const std::string& path);

        std::vector<std::shared_ptr<RayTracer::Camera>> cameras;
        std::map<std::string, std::shared_ptr<RayTracer::Camera>> cameraMap;
        std::vector<std::shared_ptr<RayTracer::IPrimitive>> primitives;
        std::vector<std::shared_ptr<RayTracer::ILight>> lights;

        // Camera and object access
        std::shared_ptr<RayTracer::Camera> getCameraByName(const std::string& name) const;
        bool moveObject(const std::string& name, const Math::Vector3D& offset);

    private:
        Core::PrimitiveFactory& _factory;
};
