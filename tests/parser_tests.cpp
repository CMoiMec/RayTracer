#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Parser/Parser.hpp"

Test(parser, valid_scene_file)
{
    Parser::Parser parser;
    bool result = parser.loadFromFile("tests/valid_scene.cfg");
    cr_assert(result, "Parser should successfully load a valid file");
    cr_assert_eq(parser.hasError(), false, "No error should be reported");

    // Check cameras
    const auto& cameras = parser.getCameras();
    cr_assert_eq(cameras.size(), 3, "The file should contain 3 cameras");
    cr_assert_eq(cameras[0].name, "main_camera", "The camera name should be 'main_camera'");
    cr_assert_eq(cameras[0].resolution.width, 800, "Width should be 800");
    cr_assert_eq(cameras[0].resolution.height, 600, "Height should be 600");

    // Check spheres
    const auto& spheres = parser.getSpheres();
    cr_assert_eq(spheres.size(), 2, "The file should contain 2 spheres");

    // Check first sphere
    cr_assert_float_eq(spheres[0].position.x, 0.0, 1e-6, "Sphere 1 position X");
    cr_assert_float_eq(spheres[0].position.y, 0.0, 1e-6, "Sphere 1 position Y");
    cr_assert_float_eq(spheres[0].position.z, -50.0, 1e-6, "Sphere 1 position Z");
    cr_assert_float_eq(spheres[0].radius, 2.0, 1e-6, "Sphere 1 radius");
    cr_assert_eq(spheres[0].color.r, 255, "Sphere 1 color R");
    cr_assert_eq(spheres[0].color.g, 0, "Sphere 1 color G");
    cr_assert_eq(spheres[0].color.b, 0, "Sphere 1 color B");

    // Check planes
    const auto& planes = parser.getPlanes();
    cr_assert_eq(planes.size(), 1, "The file should contain 1 plane");
    cr_assert_str_eq(planes[0].axis.c_str(), "Z", "The plane axis should be 'Z'");
    cr_assert_float_eq(planes[0].position, -60.0, 1e-6, "Plane position");

    // Check lights
    const auto& lights = parser.getLights();
    cr_assert_float_eq(lights.ambient, 0.3, 1e-6, "Ambient intensity");
    cr_assert_float_eq(lights.diffuse, 0.6, 1e-6, "Diffuse intensity");
    cr_assert_eq(lights.directional.size(), 1, "There should be 1 directional light");
    cr_assert_eq(lights.point.size(), 1, "There should be 1 point light");
}

Test(parser, nonexistent_file)
{
    Parser::Parser parser;

    bool result = parser.loadFromFile("tests/nonexistent_file.cfg");

    cr_assert_not(result, "Parser should fail with a nonexistent file");
    cr_assert(parser.hasError(), "An error should be reported");
}

Test(parser, invalid_syntax) {
    Parser::Parser parser;

    bool result = parser.loadFromFile("tests/bad_scene.cfg");

    cr_assert_not(result, "Parser should fail with invalid syntax");
    cr_assert(parser.hasError(), "An error should be reported");
}

Test(parser, missing_fields)
{

    Parser::Parser parser;

    bool result = parser.loadFromFile("tests/missing_fields.cfg");

    cr_assert(result, "Parser should succeed even with missing fields");

    const auto& cameras = parser.getCameras();
    cr_assert_eq(cameras.size(), 1, "The file should contain 1 camera");
    cr_assert_float_eq(cameras[0].fieldOfView, 60.0, 1e-6, "Default FOV should be 60.0");

    const auto& spheres = parser.getSpheres();
    cr_assert_eq(spheres.size(), 1, "The file should contain 1 sphere");
    cr_assert_eq(spheres[0].color.r, 255, "Default color R");
    cr_assert_eq(spheres[0].color.g, 255, "Default color G");
    cr_assert_eq(spheres[0].color.b, 255, "Default color B");
}

