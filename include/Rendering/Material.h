//
// Created by etien on 04/02/2021.
//

#ifndef CLOTHSIM_MATERIAL_H
#define CLOTHSIM_MATERIAL_H

#include <nlohmann/json.hpp>
#include "TextureManager.h"

struct Material {
    unsigned int diffuse = TextureManager::emptyTexture();
    unsigned int specular = TextureManager::emptyTexture();

    unsigned int  albedoMap = TextureManager::emptyTexture();
    unsigned int  normalMap = TextureManager::emptyTexture("default_normal_internal", 128, 128, 255);
    unsigned int  metallicMap = TextureManager::emptyTexture();
    unsigned int  roughnessMap = TextureManager::emptyTexture();
    unsigned int  aoMap = TextureManager::emptyTexture();
    unsigned int  emissionMap = TextureManager::emptyTexture("default_black_internal", 0, 0, 0);


    nlohmann::json toJson() {
        nlohmann::json json;

        json["diffuse"] = TextureManager::getName(diffuse);
        json["specular"] = TextureManager::getName(specular);
        json["albedoMap"] = TextureManager::getName(albedoMap);
        json["normalMap"] = TextureManager::getName(normalMap);
        json["metallicMap"] = TextureManager::getName(metallicMap);
        json["roughnessMap"] = TextureManager::getName(roughnessMap);
        json["aoMap"] = TextureManager::getName(aoMap);
        json["emissionMap"] = TextureManager::getName(emissionMap);

        return json;
    }

    static Material fromJson(nlohmann::json json) {
        Material material;

        material.diffuse = TextureManager::loadTexture(json["diffuse"]);
        material.specular = TextureManager::loadTexture(json["specular"]);
        material.albedoMap = TextureManager::loadTexture(json["albedoMap"]);
        material.normalMap = TextureManager::loadTexture(json["normalMap"]);
        material.metallicMap = TextureManager::loadTexture(json["metallicMap"]);
        material.roughnessMap = TextureManager::loadTexture(json["roughnessMap"]);
        material.aoMap = TextureManager::loadTexture(json["aoMap"]);
        material.emissionMap = TextureManager::loadTexture(json["emissionMap"]);

        return material;
    }
};

#endif //CLOTHSIM_MATERIAL_H
