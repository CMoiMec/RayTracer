/*
**
**
**
**
*/

#pragma once

#include <memory>
#include <limits>
#include <cmath>
#include "Renderer/Image.hpp"
#include "Core/Scene.hpp"
#include "RayTracer/HitInfo.hpp"
#include "RayTracer/Camera.hpp"
#include "RayTracer/ILight.hpp"
#include "RayTracer/AmbientLight.hpp"
#include "RayTracer/DirectionalLight.hpp"
#include "RayTracer/PointLight.hpp"

class Renderer {
    public:
        Renderer(int width, int height, int samplesPerPixel = 1);
        ~Renderer() = default;

        /**
         * \brief Render the scene from a given camera.
         * \param scene  Parsed scene holding cameras / primitives / lights
         * \param camera Active camera
         * \return       Frame-buffer ready to be saved as PPM
         */
        Image render(const Scene& scene,
        const std::shared_ptr<RayTracer::Camera>& camera) const;

    private:
        int _w;
        int _h;
        int _samplesPerPixel;

        bool tracePrimaryRay(const Scene& scene,
                         const RayTracer::Ray& ray,
                         HitInfo& outHit) const;
        Color shadePixel(const Scene& scene, const HitInfo& hit) const;
        bool isShadowed(const Scene& scene,
            const Math::Point3D& P,
            const Math::Vector3D& L,
            double maxDist = std::numeric_limits<double>::infinity()
        )const;
        static Color writeBackground();
        static void processBlocks(const Renderer* renderer, const struct ThreadData& data);
};
