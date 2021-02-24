//
// Created by etien on 06/02/2021.
//

#include <iostream>
#include <fstream>
#include <API/API3D.h>
#include <assimp/Exporter.hpp>
#include <ImGui/ImFileDialog.h>

#include "Rendering/Model.h"


/*aiScene GenerateScene(const Model& model) {

    aiScene scene;

    scene.mRootNode = new aiNode();

    scene.mMaterials = new aiMaterial*[ 1 ];
    scene.mMaterials[ 0 ] = nullptr;
    scene.mNumMaterials = 1;

    scene.mMaterials[ 0 ] = new aiMaterial();


    for (const Mesh& mesh : model.meshes) {

        scene.mMeshes = new aiMesh*[ 1 ];
        scene.mMeshes[ 0 ] = nullptr;
        scene.mNumMeshes = 1;

        scene.mMeshes[ 0 ] = new aiMesh();
        scene.mMeshes[ 0 ]->mMaterialIndex = 0;

        scene.mRootNode->mMeshes = new unsigned int[ 1 ];
        scene.mRootNode->mMeshes[ 0 ] = 0;
        scene.mRootNode->mNumMeshes = 1;


        auto pMesh = scene.mMeshes[ 0 ];



        pMesh->mVertices = new aiVector3D[ mesh.vertices.size()/3 ];
        pMesh->mNumVertices = mesh.vertices.size()/3;

        pMesh->mTextureCoords[ 0 ] = new aiVector3D[ mesh.vertices.size()/3 ];
        pMesh->mNumUVComponents[ 0 ] = mesh.vertices.size()/3;

        const auto& vVertices = mesh.vertices;

        int index = 0;
        for (auto itr = vVertices.begin(); itr != vVertices.end(); ++itr) {

            pMesh->mVertices[itr - vVertices.begin()] = aiVector3D( mesh.vertices[index*3], mesh.vertices[index*3+1], mesh.vertices[index*3+2]);
            pMesh->mNormals[itr - vVertices.begin()] = aiVector3D( mesh.normales[index*3], mesh.normales[index*3+1], mesh.normales[index*3+2]);
            pMesh->mTextureCoords[0][itr - vVertices.begin() ] = aiVector3D( mesh.texcoords[index*2], mesh.texcoords[index*2+1], 0 );

            index += 1;
        }


        pMesh->mFaces = new aiFace[mesh.vertices.size()/3/3];
        pMesh->mNumFaces = mesh.vertices.size()/3/3;

        for (int i = 0; i < mesh.vertices.size()/3/3; i++) {

            aiFace& face = pMesh->mFaces[i];

            face.mIndices = new unsigned int[ 3 ];
            face.mNumIndices = 3;

            face.mIndices[ 0 ] = mesh.indices[i];
            face.mIndices[ 1 ] = mesh.indices[i+1];
            face.mIndices[ 2 ] = mesh.indices[i]+2;
        }

    }

    return scene;
}*/



inline bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Model::Model(const std::string &path) {
    loadModel(path);
}

void Model::render(RenderingChain *renderingChain, Transform *transform) {
    for(auto& mesh : meshes)
        mesh.render(renderingChain, transform);
}

void Model::loadModel(const std::string& path) {
    name = path;

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = std::filesystem::path(path.c_str()).parent_path().string() + "/";

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {

    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    Mesh return_mesh;

    return_mesh.name = mesh->mName.C_Str();

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // process vertex positions, normals and texture coordinates
        return_mesh.vertices.push_back(mesh->mVertices[i].x);
        return_mesh.vertices.push_back(mesh->mVertices[i].y);
        return_mesh.vertices.push_back(mesh->mVertices[i].z);

        return_mesh.normales.push_back(mesh->mNormals[i].x);
        return_mesh.normales.push_back(mesh->mNormals[i].y);
        return_mesh.normales.push_back(mesh->mNormals[i].z);

        return_mesh.texcoords.push_back(mesh->mTextureCoords[0][i].x);
        return_mesh.texcoords.push_back(mesh->mTextureCoords[0][i].y);

        return_mesh.colors.insert(return_mesh.colors.begin(), {1, 1, 1});
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            return_mesh.indices.push_back(face.mIndices[j]);
    }


    // process material
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        for(const auto& map : diffuseMaps)
            textures.insert(map);
        if(!diffuseMaps.empty()) return_mesh.material.diffuse = diffuseMaps[0].id;
        if(!diffuseMaps.empty()) return_mesh.material.albedoMap = diffuseMaps[0].id;

        std::vector<Texture> specularMaps = loadMaterialTextures(material ,aiTextureType_SPECULAR, "texture_specular");
        for(const auto& map : specularMaps)
            textures.insert(map);
        if(!specularMaps.empty()) return_mesh.material.specular = specularMaps[0].id;

        std::vector<Texture> shininessMaps = loadMaterialTextures(material ,aiTextureType_SHININESS, "texture_shininess");
        for(const auto& map : shininessMaps)
            textures.insert(map);
        if(!shininessMaps.empty()) return_mesh.material.roughnessMap = shininessMaps[0].id;

        std::vector<Texture> metalnessMaps = loadMaterialTextures(material ,aiTextureType_METALNESS, "texture_metalness");
        for(const auto& map : metalnessMaps)
            textures.insert(map);
        if(!metalnessMaps.empty()) return_mesh.material.metallicMap = metalnessMaps[0].id;

        std::vector<Texture> normalMaps = loadMaterialTextures(material ,aiTextureType_NORMALS, "texture_normal");
        for(const auto& map : normalMaps)
            textures.insert(map);
        if(!normalMaps.empty()) return_mesh.material.normalMap = normalMaps[0].id;

        std::vector<Texture> aoMaps = loadMaterialTextures(material ,aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
        for(const auto& map : aoMaps)
            textures.insert(map);
        if(!aoMaps.empty()) return_mesh.material.aoMap = aoMaps[0].id;


        // For mtl : refl and norm
        if(ends_with(name, ".obj")) {
            std::string mtlpath = name.substr(0, name.size()-4) + ".mtl";
            std::ifstream file(mtlpath);
            std::string word;
            std::string current_mesh;
            while (file >> word) {
                if(word == "newmtl") {
                    file >> current_mesh;
                }
                else if(current_mesh == return_mesh.name && word == "refl") {
                    std::string texture_path;
                    file >> texture_path;
                    Texture texture;
                    texture.id = TextureManager::loadTexture(directory + texture_path);
                    texture.type = "texture_metalness";
                    texture.path = directory + texture_path;
                    textures.insert(texture);
                    return_mesh.material.metallicMap = texture.id;
                }
                else if(current_mesh == return_mesh.name && word == "norm") {
                    std::string texture_path;
                    file >> texture_path;
                    Texture texture;
                    texture.id = TextureManager::loadTexture(directory + texture_path);
                    texture.type = "texture_normal";
                    texture.path = directory + texture_path;
                    textures.insert(texture);
                    return_mesh.material.normalMap = texture.id;
                }
                else if(current_mesh == return_mesh.name && word == "map_Ka") {
                    std::string texture_path;
                    file >> texture_path;
                    Texture texture;
                    texture.id = TextureManager::loadTexture(directory + texture_path);
                    texture.type = "texture_ao";
                    texture.path = directory + texture_path;
                    textures.insert(texture);
                    return_mesh.material.aoMap = texture.id;
                }
            }
        }



    }

    return_mesh.updateBuffers();

    return return_mesh;

}

std::vector<Model::Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName) {

    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureManager::loadTexture(directory + str.C_Str());
        texture.type = typeName;
        texture.path = directory + str.C_Str();
        textures.push_back(texture);
    }
    return textures;

}

void Model::renderGUI() {

    ImGui::Text("Model %s", name.c_str());

    if(ImGui::Button("Apply textures of first mesh to all meshes") && !meshes.empty()) {
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].material = meshes[0].material;
        }
    }

    for(int i = 0; i < meshes.size(); i++) {

        if(ImGui::CollapsingHeader(("Mesh " + meshes[i].name).c_str())) {
            meshes[i].renderGUI();
        }

    }

    if(ImGui::CollapsingHeader("Textures")) {

        for(const auto& tex : textures) {
            ImGui::Text("%s [%s]", tex.path.c_str(), tex.type.c_str());
        }


        // region open texture
        if (ImGui::Button("Open a texture..."))
            ifd::FileDialog::Instance().Open("ModelOpenTexture", "Open a texture", "Image file (*.png,*.jpg){.png,.jpg},.*");

        if (ifd::FileDialog::Instance().IsDone("ModelOpenTexture")) {
            if (ifd::FileDialog::Instance().HasResult()) {
                const std::wstring& res = ifd::FileDialog::Instance().GetResult();
                std::string path;
                for(char x : res) path += x;

                Texture texture;
                texture.id = TextureManager::loadTexture(path);
                texture.type = "";
                texture.path = path;
                textures.insert(texture);
            }
            ifd::FileDialog::Instance().Close();
        }
        // endregion

    }

}

nlohmann::json Model::toJson() {
    nlohmann::json json;

    json["name"] = name;

    for(auto& mesh : meshes)
        json["meshes"].push_back(mesh.toJson());

    return json;
}

Model::~Model() {
//    for(auto& mesh : meshes) {
//        API3D::instance->freeMeshBuffers(mesh);
//    }
}

Model Model::fromJson(nlohmann::json json) {
    Model model(json["name"]);

    for(auto& jmesh : json["meshes"])
        for(auto& mesh : model.meshes)
            if(jmesh["name"] == mesh.name) {
                mesh.parseJson(jmesh);
            }

    return model;
}
