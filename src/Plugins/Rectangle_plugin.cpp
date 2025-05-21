/*
**
**
**
**
*/

#include "RayTracer/Rectangle.hpp"
#include "RayTracer/IPrimitive.hpp"
#include "Core/PrimitiveConfig.hpp"

#include <memory>

extern "C" {
    RayTracer::IPrimitive* createPrimitive(const PrimitiveConfig& conf)
    {
        RectangleData_t rect = std::get<RectangleData_t>(conf.data);
        return new RayTracer::Rectangle(rect.origin, rect.bottom, rect.left, conf.color);
    }

    const char *getType()
    {
        return "rectangle";
    }
}
