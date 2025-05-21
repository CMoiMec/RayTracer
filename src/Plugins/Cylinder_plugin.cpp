/*
**
**
**
**
*/

#include "RayTracer/Cylinder.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"

#include <memory>

extern "C" {
    RayTracer::IPrimitive* createPrimitive(const PrimitiveConfig& conf)
    {
        CylinderData_t cylindre = std::get<CylinderData_t>(conf.data);
        return new RayTracer::Cylinder(cylindre.baseCenter, cylindre.axis, cylindre.radius, cylindre.height, conf.color);
    }

    const char *getType()
    {
        return "cylinder";
    }
}

