//
// Created by Scott on 12/07/2020.
//

#include <iostream>
#include "TextureManager.h"

std::unordered_map<std::string, sf::Texture> TextureManager::map;

bool TextureManager::loadFromFile(const std::string& filePath, const std::string& name) {

    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        return false;
    }

    map[name] = texture;

    return true;
}

sf::Texture *TextureManager::getTexture(const std::string& name) {

    auto texture = &map[name];

    if (texture == nullptr) {
        std::cout << "Failed to find the texture with the name: " << name << " please ensure it is loaded" << std::endl;
    }

    return texture;
}
