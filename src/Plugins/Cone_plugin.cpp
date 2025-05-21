/*
**
**
**
**
*/

#include "RayTracer/Cone.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"

#include <iostream>

extern "C" {
    RayTracer::IPrimitive* createPrimitive(const PrimitiveConfig& conf)
    {
        ConeData_t cone = std::get<ConeData_t>(conf.data);
        return new RayTracer::Cone(cone.apex, cone.axis, cone.radius, cone.height, conf.color);
    }

    const char *getType()
    {
        return "cone";
    }
}
