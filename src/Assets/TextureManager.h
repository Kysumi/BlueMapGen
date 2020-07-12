//
// Created by Scott on 12/07/2020.
//

#ifndef ONGRID_TEXTUREMANAGER_H
#define ONGRID_TEXTUREMANAGER_H

#include <string>
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

class TextureManager {
public:
    static bool loadFromFile(const std::string& filePath, const std::string& name);
    static sf::Texture* getTexture(const std::string& name);
private:
    static std::unordered_map<std::string, sf::Texture> map;
};


#endif //ONGRID_TEXTUREMANAGER_H
