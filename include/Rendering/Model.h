//
// Created by etien on 06/02/2021.
//

#ifndef CLOTHSIM_MODEL_H
#define CLOTHSIM_MODEL_H

#include <string>
#include <set>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Rendering/Mesh.h"

class Model {

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
        bool operator==(const Texture& other) const {
            return id == other.id;
        }
        bool operator<(const Texture& other) const {
            return id < other.id;
        }
    };

public:
    Model() = default;
    Model(const std::string& path);

    virtual ~Model();

    void render(RenderingChain* renderingChain, Transform* transform);

    void renderGUI();

    std::string name;
    std::vector<Mesh> meshes;

    nlohmann::json toJson();
    static Model fromJson(nlohmann::json json);

private:
    // model data
    std::set<Texture> textures;

    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);

};


#endif //CLOTHSIM_MODEL_H
