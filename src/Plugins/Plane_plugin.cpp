/*
**
**
**
**
*/

#include "RayTracer/Plane.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"

#include <memory>

extern "C" {
    RayTracer::IPrimitive* createPrimitive(const PrimitiveConfig& conf)
    {
        PlaneData_t plane = std::get<PlaneData_t>(conf.data);
        return new RayTracer::Plane(plane.pos, plane.norm, conf.color);
    }

    const char *getType()
    {
        return "plane";
    }
}
