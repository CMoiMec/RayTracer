/*
** CompositePrimitive - Groups multiple primitives into a single object
**
*/
#pragma once
#include <vector>
#include <memory>
#include "RayTracer/IPrimitive.hpp"
#include "Utils/Color.hpp"
#include "RayTracer/HitInfo.hpp"

namespace RayTracer {
    /**
     * @brief Groups multiple primitives into a single object
     */
    class CompositePrimitive : public IPrimitive {
    private:
        std::vector<std::shared_ptr<IPrimitive>> m_children;
        Color m_color;

    public:
        /**
         * @brief Creates a composite containing multiple primitives with the specified color
         * @param color The color to use for this composite
         */
        CompositePrimitive(const Color& color = Color(255, 255, 255));

        /**
         * @brief Destructor
         */
        ~CompositePrimitive() = default;

        /**
         * @brief Adds a primitive to this composite
         * @param child The primitive to add
         */
        void addChild(std::shared_ptr<IPrimitive> child);

        /**
         * @brief Checks if a ray hits any primitive in this composite
         * @param ray The ray to test for intersection
         * @param info Output parameter for hit information
         * @return True if hit, with info about the closest intersection
         */
        bool hits(const Ray& ray, HitInfo& info) const override;

        /**
         * @brief Returns the color of this composite
         * @return The color
         */
        const Color& getColor() const override;

        /**
         * @brief Returns the number of child primitives
         * @return Number of children
         */
        size_t getChildCount() const;
    };
}
