/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** CommandLineInterface
*/

#include "UI/CommandLineInterface.hpp"
#include "UI/SFMLViewer.hpp"
#include <iostream>
#include <sstream>

CommandLineInterface::CommandLineInterface(Scene& scene, Renderer& renderer)
    : _scene(scene), _renderer(renderer)
{
    _activeCamera = _scene.getCameraByName("main_camera");
    initCommands();
}

void CommandLineInterface::initCommands() {
    _commands["cam"] = [this](std::istringstream& iss) { cmd_cam(iss); };
    _commands["render"] = [this](std::istringstream& iss) { cmd_render(iss); };
    _commands["move"] = [this](std::istringstream& iss) { cmd_move(iss); };
    _commands["preview"] = [this](std::istringstream& iss) { cmd_preview(iss); };
}

void CommandLineInterface::run() {
    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        if (line == "exit") break;
        executeCommand(line);
        std::cout << "> ";
    }
}

void CommandLineInterface::executeCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    auto it = _commands.find(cmd);
    if (it != _commands.end()) {
        it->second(iss);
    } else
        std::cerr << "Unknown command: " << cmd << "\n";
}

void CommandLineInterface::cmd_move(std::istringstream& iss) {
    std::string name;
    float dx, dy, dz;
    if (!(iss >> name >> dx >> dy >> dz)) {
        std::cerr << "Usage: move <object_name> <dx> <dy> <dz>\n";
        return;
    }

    if (!_scene.moveObject(name, {dx, dy, dz})) {
        std::cerr << "Error: no object '" << name << "'\n";
        return;
    }

    std::cout << "Object '" << name << "' moved by (" << dx << ", " << dy << ", " << dz << ")\n";
}

void CommandLineInterface::cmd_cam(std::istringstream& iss) {
    std::string camName;
    if (!(iss >> camName)) {
        std::cerr << "Usage: cam <camera_name>\n";
        return;
    }
    auto cam = _scene.getCameraByName(camName);
    if (cam) {
        _activeCamera = cam;
        std::cout << "Camera changed to '" << camName << "'\n";
    } else
        std::cerr << "Error: No camera '" << camName << "'\n";
}

void CommandLineInterface::cmd_render(std::istringstream& iss) {
    std::string filename = "output.ppm";
    iss >> filename;
    auto frame = _renderer.render(_scene, _activeCamera);
    frame.writePPM("screenshots/" + filename);
    std::cout << "Image saved to screenshots/" << filename << "\n";
}

void CommandLineInterface::cmd_preview(std::istringstream& iss) {
    Image frame = _renderer.render(_scene, _activeCamera);
    SFMLViewer display(_activeCamera->_width, _activeCamera->_height);
    display.show(frame);
}
