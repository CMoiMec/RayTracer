/*
** PrimitiveFactory - Creates and manages geometric primitives
*/

#pragma once

#include <string>
#include <map>
#include <functional>
#include <vector>
#include <memory>

#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"
#include "Core/PrimitiveFactoryExceptions.hpp"
#include <filesystem>
#include <dlfcn.h>
#include <iostream>


namespace Core {
    /**
     * @brief Factory for creating geometric primitives using direct methods or plugins
     */
    class PrimitiveFactory {
        public:
            using CreateFn = std::function<std::shared_ptr<RayTracer::IPrimitive>(const PrimitiveConfig&)>;

            PrimitiveFactory();
            ~PrimitiveFactory();

            /**
             * @brief Loads primitive plugins from a directory
             * @param pluginsDir Directory containing the .so plugin files
             * @throws PluginLoadException if a plugin fails to load
             */
            void loadPlugins(const std::string& pluginsDir);

            /**
             * @brief Registers a new primitive type with its creation function
             * @param name Name/type identifier for the primitive
             * @param fn Function to create instances of this primitive
             */
            void registerType(const std::string& name, CreateFn fn);

            /**
             * @brief Creates a primitive using the registered type and configuration
             * @param type The type identifier of the primitive
             * @param cfg Configuration with primitive-specific data
             * @return Shared pointer to the created primitive
             * @throws UnknownPrimitiveTypeException if type is not registered
             */
            std::shared_ptr<RayTracer::IPrimitive> create(
                const std::string& type,
                const PrimitiveConfig& cfg) const;

            /**
             * @brief Creates a sphere primitive
             * @param center Center point of the sphere
             * @param radius Radius of the sphere
             * @param color Color of the sphere
             * @return Shared pointer to the created sphere
             */
            std::shared_ptr<RayTracer::IPrimitive> createSphere(
                const Math::Point3D& center,
                double radius,
                const Color& color);

            /**
             * @brief Creates a plane primitive
             * @param point Point on the plane
             * @param normal Normal vector to the plane (will be normalized)
             * @param color Color of the plane
             * @return Shared pointer to the created plane
             */
            std::shared_ptr<RayTracer::IPrimitive> createPlane(
                const Math::Point3D& point,
                const Math::Vector3D& normal,
                const Color& color);

            /**
             * @brief Creates a cone primitive
             * @param apex Top point of the cone
             * @param axis Direction from apex to base (will be normalized)
             * @param radius Radius of the base
             * @param height Height of the cone
             * @param color Color of the cone
             * @return Shared pointer to the created cone
             */
            std::shared_ptr<RayTracer::IPrimitive> createCone(
                const Math::Point3D& apex,
                const Math::Vector3D& axis,
                double radius,
                double height,
                const Color& color);

            /**
             * @brief Creates a cylinder primitive
             * @param baseCenter Center of the bottom circular face
             * @param axis Direction from base to top (will be normalized)
             * @param radius Radius of the cylinder
             * @param height Height of the cylinder
             * @param color Color of the cylinder
             * @return Shared pointer to the created cylinder
             */
            std::shared_ptr<RayTracer::IPrimitive> createCylinder(
                const Math::Point3D& baseCenter,
                const Math::Vector3D& axis,
                double radius,
                double height,
                const Color& color);

        private:
            struct PluginHandle { void* handle; };
            std::vector<PluginHandle> _handles;
            std::map<std::string, CreateFn> _creators;
    };
}
