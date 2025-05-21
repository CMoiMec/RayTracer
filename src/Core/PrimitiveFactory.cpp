/*
** PrimitiveFactory - Implementation of creation methods and plugin loading
**
*/

#include "Core/PrimitiveFactory.hpp"
#include "RayTracer/Sphere.hpp"
#include "RayTracer/Plane.hpp"
#include "RayTracer/Cone.hpp"
#include "RayTracer/Cylinder.hpp"

Core::PrimitiveFactory::PrimitiveFactory()
{
}

Core::PrimitiveFactory::~PrimitiveFactory()
{
    for (auto& ph : _handles) {
        if (ph.handle)
            dlclose(ph.handle);
    }
}

void Core::PrimitiveFactory::loadPlugins(const std::string& pluginsDir)
{
    if (!std::filesystem::exists(pluginsDir) ||
    !std::filesystem::is_directory(pluginsDir)) {
        throw PluginLoadException(pluginsDir, "not or no a directory");
    }

    for (auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (entry.path().extension() != ".so") continue;

        std::string path = entry.path().string();
        void* h = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
        if (!h) {
            throw PluginLoadException(path, dlerror());
        }

        using NameFn   = const char*();
        using CreateFnC= RayTracer::IPrimitive*(const PrimitiveConfig&);
        auto getName = reinterpret_cast<NameFn*>(dlsym(h, "getType"));
        auto create  = reinterpret_cast<CreateFnC*>(dlsym(h, "createPrimitive"));
        if (!getName || !create) {
            dlclose(h);
            throw PluginLoadException(
                path,
                "missing symbol getPrimitiveName or createPrimitive"
            );
        }

        std::string name = getName();
        _creators[name] = [create](const PrimitiveConfig& cfg){
            return std::shared_ptr<RayTracer::IPrimitive>( create(cfg) );
        };
        _handles.push_back({h});
        std::cout << "[Plugin] Loaded \"" << name << "\" from " << path << "\n";
    }
}

void Core::PrimitiveFactory::registerType(const std::string& name, CreateFn fn)
{
     _creators[name] = std::move(fn);
}

std::shared_ptr<RayTracer::IPrimitive> Core::PrimitiveFactory::create(
        const std::string& type,
        const PrimitiveConfig& cfg) const
{
    auto it = _creators.find(type);
    if (it == _creators.end())
        throw UnknownPrimitiveTypeException(type);
    return it->second(cfg);
}
