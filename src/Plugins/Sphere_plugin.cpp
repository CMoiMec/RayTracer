/*
**
**
**
**
*/

#include "RayTracer/Sphere.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"

#include <memory>
#include <iostream>

extern "C" {
    RayTracer::IPrimitive* createPrimitive(const PrimitiveConfig& conf)
    {
        SphereData_t sphere = std::get<SphereData_t>(conf.data);
        return new RayTracer::Sphere(sphere.c, sphere.r, conf.color);
    }

    const char *getType()
    {
        return "sphere";
    }
}
