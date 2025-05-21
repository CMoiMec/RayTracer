/*
**
**
**
**
*/

#include "RayTracer/Triangle.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"

#include <memory>

extern "C" {
    RayTracer::IPrimitive* createPrimitive(const PrimitiveConfig& conf)
    {
        TriangleData_t triangle = std::get<TriangleData_t>(conf.data);
        return new RayTracer::Triangle(triangle.a, triangle.b, triangle.c, conf.color);
    }

    const char *getType()
    {
        return "triangle";
    }
}
