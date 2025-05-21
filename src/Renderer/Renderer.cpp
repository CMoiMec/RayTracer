/*
** Renderer.cpp - Implementation of multithreaded raytracer
*/

#include "Renderer/Renderer.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <atomic>
#include <iostream>

/**
 * @brief Constructor for the renderer
 * @param w Width of the output image in pixels
 * @param h Height of the output image in pixels
 */
Renderer::Renderer(int w, int h, int samplesPerPixel) : _w(w), _h(h), _samplesPerPixel(samplesPerPixel) {}

// Structure to hold shared rendering data using references to avoid const issues
struct ThreadData {
    const Scene& scene;
    const std::shared_ptr<RayTracer::Camera>& camera;
    Image& frame;
    int blockSize;
    int width;
    int height;
    std::queue<std::pair<int, int>>& blockQueue;
    std::mutex& queueMutex;
    std::mutex& frameMutex;
    std::atomic<int>& blocksCompleted;
    int totalBlocks;
};

/**
 * @brief Worker function to process blocks of the image
 * @param renderer Pointer to the renderer
 * @param data Thread shared data
 */
void Renderer::processBlocks(const Renderer* renderer, const ThreadData& data) {
    while (true) {
        // Get next block to render (thread-safe)
        std::pair<int, int> block;
        {
            std::lock_guard<std::mutex> lock(data.queueMutex);
            if (data.blockQueue.empty()) {
                break; // No more blocks to process
            }
            block = data.blockQueue.front();
            data.blockQueue.pop();
        }

        // Calculate this block's coordinates
        int blockX = block.first;
        int blockY = block.second;
        int startX = blockX * data.blockSize;
        int startY = blockY * data.blockSize;
        int endX = std::min(startX + data.blockSize, data.width);
        int endY = std::min(startY + data.blockSize, data.height);

        // Use local buffer to minimize lock time
        std::vector<Color> blockBuffer((endX - startX) * (endY - startY));

        // Calculate the size of the sampling grid
        int gridSize = static_cast<int>(std::sqrt(renderer->_samplesPerPixel));
        // Render all pixels in this block
        for (int y = startY; y < endY; ++y) {
            double v = static_cast<double>(y) / (data.height - 1);
            for (int x = startX; x < endX; ++x) {
                double u = static_cast<double>(x) / (data.width - 1);
                RayTracer::Ray ray = data.camera->ray(u, v);

                HitInfo hit;
                Color::Float pixel(0.f, 0.f, 0.f);
                // shoot multiple rays per pixel for antialiasing
                for (int s = 0; s < renderer->_samplesPerPixel; ++s) {
                    int sx = s % gridSize;
                    int sy = s / gridSize;
                    double offsetU = (sx + 0.5) / gridSize - 0.5;
                    double offsetV = (sy + 0.5) / gridSize - 0.5;
                    double u = (x + 0.5 + offsetU) / (data.width - 1);
                    double v = (y + 0.5 + offsetV) / (data.height - 1);

                    RayTracer::Ray ray = data.camera->ray(u, v);
                    HitInfo hit;
                    Color sampleColor;
                    // Trace the ray into the scene and determine the color
                    if (renderer->tracePrimaryRay(data.scene, ray, hit))
                        sampleColor = renderer->shadePixel(data.scene, hit);
                    else
                        sampleColor = renderer->writeBackground();
                    pixel += Color::Float(sampleColor);
                }

                // Store in local buffer
                int localX = x - startX;
                int localY = y - startY;
                Color finalColor = (pixel * (1.0f / renderer->_samplesPerPixel)).toColor();
                blockBuffer[localY * (endX - startX) + localX] = finalColor;
            }
        }

        {
            std::lock_guard<std::mutex> lock(data.frameMutex);
            for (int y = startY; y < endY; ++y) {
                for (int x = startX; x < endX; ++x) {
                    int localX = x - startX;
                    int localY = y - startY;
                    data.frame.setPixel(x, data.height - 1 - y,
                                       blockBuffer[localY * (endX - startX) + localX]);
                }
            }
        }

        int completed = ++(data.blocksCompleted);
        if (completed % 10 == 0 || completed == data.totalBlocks) {
            float progress = 100.0f * completed / data.totalBlocks;
            std::cout << "\rRendering progress: " << progress << "% ("
                      << completed << "/" << data.totalBlocks << " blocks)" << std::flush;
        }
    }
}

/**
 * @brief Renders a scene using multithreaded block-based approach
 * @param scene The scene to render
 * @param cam The camera defining the viewpoint
 * @return The rendered image
 */
Image Renderer::render(const Scene& scene,
                      const std::shared_ptr<RayTracer::Camera>& cam) const
{
    Image frame(_w, _h);

    // Divide image into blocks for parallel processing
    const int blockSize = 32;
    const int numBlocksX = (_w + blockSize - 1) / blockSize;
    const int numBlocksY = (_h + blockSize - 1) / blockSize;
    const int totalBlocks = numBlocksX * numBlocksY;

    // Thread coordination tools
    std::queue<std::pair<int, int>> blockQueue;
    std::mutex queueMutex;        // Protects the block queue
    std::mutex frameMutex;        // Protects the final image
    std::atomic<int> blocksCompleted(0);

    // Fill queue with all blocks to render
    for (int blockY = 0; blockY < numBlocksY; ++blockY) {
        for (int blockX = 0; blockX < numBlocksX; ++blockX) {
            blockQueue.push({blockX, blockY});
        }
    }

    // Set up thread data (using references to handle const correctness)
    ThreadData threadData = {
        scene,
        cam,
        frame,
        blockSize,
        _w,
        _h,
        blockQueue,
        queueMutex,
        frameMutex,
        blocksCompleted,
        totalBlocks
    };

    // Start worker threads (one per CPU core)
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    std::cout << "Rendering with " << numThreads << " threads..." << std::endl;

    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&Renderer::processBlocks, this, std::ref(threadData));
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "\nRendering complete!" << std::endl;
    return frame;
}

bool Renderer::tracePrimaryRay(const Scene& scene,
                             const RayTracer::Ray& ray,
                             HitInfo& outHit) const
{
    bool touched = false;
    outHit.t = std::numeric_limits<double>::max();
    for (const auto& prim : scene.primitives) {
        HitInfo tmp;
        if (prim->hits(ray, tmp) && tmp.t < outHit.t) {
            outHit = tmp;
            touched = true;
        }
    }
    return touched;
}

Color Renderer::shadePixel(const Scene& scene,
                          const HitInfo& hit) const
{
    Color result(0,0,0);
    for (const auto& lightPtr : scene.lights) {
        // Lumière AMBIANTE
        if (auto amb = std::dynamic_pointer_cast<RayTracer::AmbientLight>(lightPtr)) {
            result += (*hit.color) * amb->getColor() * amb->getIntensity();
            continue;
        }
        // ---------- Directionnelle ----------
        if (auto dir = std::dynamic_pointer_cast<RayTracer::DirectionalLight>(lightPtr)) {
            Math::Vector3D L = -dir->getDirection();
            if (!isShadowed(scene, hit.p, L)) {
                double diff = std::max(0.0, hit.n.dot(L));
                result += (*hit.color) * dir->getColor() * (dir->getIntensity() * diff);
            }
            continue;
        }
        // ---------- Ponctuelle ----------
        if (auto pt = std::dynamic_pointer_cast<RayTracer::PointLight>(lightPtr)) {
            Math::Vector3D L(hit.p, pt->getPosition());
            double dist2 = L.length2();
            L = L.normalize();
            if (!isShadowed(scene, hit.p, L, std::sqrt(dist2))) {
                double diff   = std::max(0.0, hit.n.dot(L));
                double atten  = 1.0 / dist2;            // atténuation simple
                result += (*hit.color) * pt->getColor()
                          * (pt->getIntensity() * diff * atten);
            }
        }
    }
    return result;
}

bool Renderer::isShadowed(const Scene& scene,
                        const Math::Point3D& p,
                        const Math::Vector3D& L,
                        double maxDist) const
{
    RayTracer::Ray shadowRay(p + L * 1e-3, L);
    for (const auto& prim : scene.primitives) {
        HitInfo tmp;
        if (prim->hits(shadowRay, tmp) && tmp.t < maxDist)
            return true;          // quelque chose bloque
    }
    return false;
}

Color Renderer::writeBackground()
{
    return Color(40,40,80);
}
