/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** SFMLViewer
*/

#include "UI/SFMLViewer.hpp"

SFMLViewer::SFMLViewer(unsigned width, unsigned height)
    : _width(width), _height(height), _window(sf::VideoMode(width, height), "Raytracer")
{
    _texture.create(width, height);
    _sprite.setTexture(_texture);
}

void SFMLViewer::show(const Image& image)
{
    std::vector<uint8_t> pixels = image.toRGBA();

    _texture.update(pixels.data());

    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window.close();
        }

        _window.clear();
        _window.draw(_sprite);
        _window.display();
    }
}
