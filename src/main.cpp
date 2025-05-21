/*
**
**
**
**
*/

#include <iostream>
#include "Core/Scene.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Image.hpp"
#include "RayTracer/Camera.hpp"
#include "Core/PrimitiveFactory.hpp"
#include "UI/CommandLineInterface.hpp"

int main(int ac, char** av)
{
    if (ac < 2) {
        std::cerr << "Usage: ./raytracer <scene.cfg>\n";
        return 84;
    }
    Core::PrimitiveFactory factory;
    factory.loadPlugins("plugins");

    Scene scene(factory);
    try {
        scene.loadFromFile(av[1]);
    } catch (const std::exception& e) {
        std::cerr << "Scene load error: " << e.what() << "\n";
        return 84;
    }

    auto cam = scene.getCameraByName("main_camera");
    if (!cam) {
        std::cerr << "No camera named \"main_camera\" in scene\n";
        return 84;
    }

    Renderer renderer(cam->_width, cam->_height, 16);
    CommandLineInterface cli(scene, renderer);
    cli.run();

    return 0;
}
