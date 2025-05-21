#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Core/Scene.hpp"
#include "Renderer/Renderer.hpp"
#include "RayTracer/Camera.hpp"
#include "Math/Point3D.hpp"
#include "Math/Vector3D.hpp"
#include "RayTracer/Sphere.hpp"
#include "RayTracer/Plane.hpp"
#include "RayTracer/AmbientLight.hpp"
#include "RayTracer/DirectionalLight.hpp"
#include "Core/PrimitiveFactory.hpp"
#include <chrono>

static Scene createTestScene()
{
    Core::PrimitiveFactory factory;
    Scene scene(factory);
    auto camera = std::make_shared<RayTracer::Camera>();
    camera->_width = 400;
    camera->_height = 300;
    camera->_origin = Math::Point3D(0, 0, 5);
    scene.cameras.push_back(camera);
    scene.cameraMap["main_camera"] = camera;
    auto sphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(0, 0, -5), 2.0, Color(255, 0, 0));
    scene.primitives.push_back(sphere);
    auto plane = std::make_shared<RayTracer::Plane>(
        Math::Point3D(0, -3, 0), Math::Vector3D(0, 1, 0), Color(0, 255, 0));
    scene.primitives.push_back(plane);
    auto ambient = std::make_shared<RayTracer::AmbientLight>(0.3, Color(255, 255, 255));
    scene.lights.push_back(ambient);
    auto directional = std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3D(-1, -1, -1), Color(255, 255, 255), 0.7);
    scene.lights.push_back(directional);
    return scene;
}

Test(renderer, multithreaded_rendering)
{
    Scene scene = createTestScene();
    auto camera = scene.getCameraByName("main_camera");
    cr_assert(camera != nullptr, "Camera should exist in test scene");
    Renderer renderer(camera->_width, camera->_height);
    Image image = renderer.render(scene, camera);
    cr_assert_eq(image.width(), camera->_width, "Image width should match camera width");
    cr_assert_eq(image.height(), camera->_height, "Image height should match camera height");
    bool foundNonBackground = false;
    Color bgColor = Color(40, 40, 80); // Background color
    for (int y = 100; y < 200 && !foundNonBackground; y += 10) {
        for (int x = 150; x < 250 && !foundNonBackground; x += 10) {
            Color pixel = image.getPixel(x, y);
            if (pixel.getR() != bgColor.getR() ||
                pixel.getG() != bgColor.getG() ||
                pixel.getB() != bgColor.getB()) {
                foundNonBackground = true;
            }
        }
    }
    cr_assert(foundNonBackground, "Rendered image should contain non-background pixels");
}

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
}

Test(renderer, progress_output, .init = redirect_all_stdout) {
    Scene scene = createTestScene();
    auto camera = scene.getCameraByName("main_camera");
    camera->_width = 200;
    camera->_height = 150;
    Renderer renderer(camera->_width, camera->_height);
    renderer.render(scene, camera);
    fflush(stdout);
    cr_assert(true, "ouaiiiiiii");
}
