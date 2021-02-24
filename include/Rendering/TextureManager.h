//
// Created by etien on 05/02/2021.
//

#ifndef CLOTHSIM_TEXTUREMANAGER_H
#define CLOTHSIM_TEXTUREMANAGER_H

#include <map>
#include <string>

class TextureManager {

    static std::map<std::string, unsigned int> loadedTextures;
    static std::map<unsigned int, std::string> reverseForNames;


public:

    static constexpr const char* TEXTURE_PATH = "resources/Textures/";

    static void loadAll(const std::string& path = TEXTURE_PATH);

    static unsigned int loadTexture(const std::string& path);

    static bool exists(const std::string& path);

    static unsigned int emptyTexture(std::string name = "default_white_internal", unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);

    static const std::string& getName(unsigned int i);

    static const std::map<unsigned int, std::string> &getReverseForNames();
};


#endif //CLOTHSIM_TEXTUREMANAGER_H
