/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** CommandLineInterface
*/

#pragma once

#include <string>
#include "Core/Scene.hpp"
#include "Renderer/Renderer.hpp"
#include <functional>

/// @class CommandLineInterface
/// @brief Handles user interaction with the raytracer through a command-line interface.
class CommandLineInterface {
public:
    /// @brief Constructs the CLI with a given scene and renderer.
    CommandLineInterface(Scene& scene, Renderer& renderer);
    /// @brief Starts the command-line interface loop and handles user input.
    void run();

private:
    Scene& _scene;
    Renderer& _renderer; ///< Reference to the renderer used to generate images.
    std::shared_ptr<RayTracer::Camera> _activeCamera; /// Currently active camera in the scene.

    /// @brief Parses and executes a command line input.
    /// @param line The command entered by the user.
    void executeCommand(const std::string& line);

    /// @brief Maps command strings to their corresponding handler functions.
    std::map<std::string, std::function<void(std::istringstream&)>> _commands;

    /// @brief Initializes the available commands in the CLI.
    void initCommands();

    // Handle the differents commands
    void cmd_cam(std::istringstream&);
    void cmd_render(std::istringstream&);
    void cmd_move(std::istringstream&);
    void cmd_preview(std::istringstream&);
};
