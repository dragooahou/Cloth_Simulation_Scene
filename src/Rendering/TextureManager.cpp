//
// Created by etien on 05/02/2021.
//

#include <GL/gl3w.h>
#include <iostream>
#include <filesystem>

#include "Rendering/TextureManager.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "Rendering/stb_image.h"

std::map<std::string, unsigned int> TextureManager::loadedTextures;
std::map<unsigned int, std::string> TextureManager::reverseForNames;

unsigned int TextureManager::loadTexture(const std::string& path) {

    if(exists(path)) {
        return loadedTextures[path];
    }

//    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    int format = GL_RGB;
    switch (nrChannels) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 4:
            format = GL_RGBA;
    }
    

    if(!data) {
        std::cerr << "Failed to load texture " << path  << " : " << stbi_failure_reason() << std::endl;
        return -1;
    }

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    if(width > maxTextureSize || height > maxTextureSize) {
        std::cerr << "Failed to load texture " << path << " : texture too large" << std::endl;
        return -1;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    loadedTextures[path] = texture;
    reverseForNames[texture] = path;

    stbi_image_free(data);

    return texture;
}

unsigned int TextureManager::emptyTexture(std::string name, unsigned char r, unsigned char g, unsigned char b) {

    if(exists(name)) {
        return loadedTextures[name];
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char data[3] = {r, g, b};

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &data);
    glGenerateMipmap(GL_TEXTURE_2D);

    loadedTextures[name] = texture;
    reverseForNames[texture] = name;

    return texture;

}

std::string no_texture = "no texture";

const std::string &TextureManager::getName(unsigned int i) {
    if(reverseForNames.find(i) == reverseForNames.end())
        return no_texture;
    return reverseForNames.at(i);
}

const std::map<unsigned int, std::string> &TextureManager::getReverseForNames() {
    return reverseForNames;
}

void TextureManager::loadAll(const std::string& path) {

    emptyTexture();

    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        if(entry.is_directory())
            loadAll(entry.path().string());
        else
            loadTexture(entry.path().string());
    }

}

bool TextureManager::exists(const std::string &path) {
    return loadedTextures.find(path) != loadedTextures.end();
}
