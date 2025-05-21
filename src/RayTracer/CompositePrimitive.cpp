#include "RayTracer/CompositePrimitive.hpp"

namespace RayTracer {

CompositePrimitive::CompositePrimitive(const Color& color)
    : m_color(color)
{
}

void CompositePrimitive::addChild(std::shared_ptr<IPrimitive> child)
{
    m_children.push_back(child);
}

bool CompositePrimitive::hits(const Ray& ray, HitInfo& info) const
{
    bool hitAnything = false;
    HitInfo closestHit;
    closestHit.t = std::numeric_limits<double>::infinity();

    for (const auto& child : m_children) {
        HitInfo tmp;
        if (child->hits(ray, tmp) && tmp.t < closestHit.t) {
            closestHit = tmp;
            hitAnything = true;
        }
    }

    if (!hitAnything)
        return false;

    info = closestHit;
    info.color = &m_color;
    return true;
}

const Color& CompositePrimitive::getColor() const
{
    return m_color;
}

size_t CompositePrimitive::getChildCount() const
{
    return m_children.size();
}

}
