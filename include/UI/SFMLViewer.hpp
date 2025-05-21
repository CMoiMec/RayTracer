/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** SFMLViewer
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer/Image.hpp"

/// @class SFMLViewer
/// @brief A viewer class that uses SFML to display a rendered image.
class SFMLViewer {
public:
    /// @brief Constructs an SFMLViewer with the given dimensions.
    SFMLViewer(unsigned width, unsigned height);
    /// @brief Displays the given image in the SFML window.
    void show(const Image& image);

private:

    unsigned _width, _height;
    sf::RenderWindow _window;
    sf::Texture _texture;
    sf::Sprite _sprite;
};
