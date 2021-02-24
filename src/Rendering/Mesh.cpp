#include <iostream>
#include <API/API3D.h>

#include "Rendering/Mesh.h"
#include "Rendering/Camera.h"

Mesh::Mesh() = default;

// Ajouter un vertex et une couleur dans l'objet
void Mesh::addVertex(float x, float y, float z, float nx, float ny, float nz, float txx, float txy) {
    vertices.insert(vertices.end(), {x, y, z});
    colors.insert(colors.end(), {color.x, color.y, color.z});
    normales.insert(normales.end(), {nx, ny, nz});
    texcoords.insert(texcoords.end(), {txx, txy});
}

// Ajouter un vertex et une couleur dans l'objet
void Mesh::addVertex(Vec3 v, Vec3 normal, Vec3 texCoord) {
    addVertex(v.x, v.y, v.z, normal.x, normal.y, normal.z, texCoord.x, texCoord.y);
}

// Changer la couleur des prochains vertex à ajouter
void Mesh::setColor(float r, float g, float b) {
    color.x = r;
    color.y = g;
    color.z = b;
}

const std::vector<float> &Mesh::getVertices() const {
    return vertices;
}

const std::vector<float> &Mesh::getColors() const {
    return colors;
}

// Dessiner l'objet 3D (drawingType peut être égal à GL_QUADS, GL_LINES, etc...)
void Mesh::render(RenderingChain* renderChain, Transform* transform) {

    renderChain->addStep([this, transform](Camera* camera){
        Transform t;
        if(transform != nullptr)
            t = *transform;
        API3D::instance->setBasePVM(camera->projectionMat, camera->viewMat, reinterpret_cast<const glm::mat4 &>(t));
        float useLightf = useLight && camera->useLighting;
        API3D::instance->uniform1fv(API3D::instance->getBaseShader(), "useLight", &useLightf, 1);

        API3D::instance->renderMesh(*this);
    });
}

// Retourne le nombre de vertex
size_t Mesh::getVertexCount() {
    return vertices.size()/3;
}

// Ajoute un quad à l'objet
void Mesh::addQuad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, float mergeDist) {

    // Les indices des vertex dans le tableau "vertices"
    int ia = -1, ib = -1, ic = -1, id = -1;
    mergeDist *= mergeDist;

    // On vérifie qu'on peux fusionner ou non des vertices
    // On check les 100 derniers (pas oublier de multiplier de 3 mdr)
    int checkLast = 100*3;
    // L'indice auquel un démarre
    int starti = std::max(0ull, (long long unsigned int)vertices.size()-checkLast);

    // Pour chaque vertex à vérifier
    for(int i = starti; i < vertices.size(); i+=3) {
        Vec3 v = {vertices[i], vertices[i+1], vertices[i+2]};

        // a
        if(ia < 0)
            if(v.distSq(a) < mergeDist) {
                ia = i/3;
            }

        // b
        if(ib < 0)
            if(v.distSq(b) < mergeDist) {
                ib = i/3;
            }

        // c
        if(ic < 0)
            if(v.distSq(c) < mergeDist) {
                ic = i/3;
            }

        // d
        if(id < 0)
            if(v.distSq(d) < mergeDist) {
                id = i/3;
            }

    }

    // Si on a pas pu fusionner, on le crée
    if(ia < 0){
        ia = vertices.size()/3;
        addVertex(a, na);
        
    }
    if(ib < 0){
        ib = vertices.size()/3;
        addVertex(b, nb);
    }
    if(ic < 0){
        ic = vertices.size()/3;
        addVertex(c, nc);
    }
    if(id < 0){
        id = vertices.size()/3;
        addVertex(d, nd);
    }

    // On push les indices
    // Premier triangle
    indices.push_back(ia);
    indices.push_back(ib);
    indices.push_back(ic);
    // Second triangle
    indices.push_back(ia);
    indices.push_back(ic);
    indices.push_back(id);


}

void Mesh::addLine(Vec3 a, Vec3 b, Vec3 na, Vec3 nb, float mergeDist) {
    // Les indices des vertex dans le tableau "vertices"
    int ia = -1, ib = -1;
    mergeDist *= mergeDist;

    // On vérifie qu'on peux fusionner ou non des vertices
    // On check les 100 derniers (pas oublier de multiplier de 3 mdr)
    int checkLast = 100*3;
    // L'indice auquel un démarre
    int starti = std::max(0ull, (long long unsigned int)vertices.size()-checkLast);

    // Pour chaque vertex à vérifier
    for(int i = starti; i < vertices.size(); i+=3) {
        Vec3 v = {vertices[i], vertices[i+1], vertices[i+2]};

        // a
        if(ia < 0)
            if(v.distSq(a) < mergeDist) {
                ia = i/3;
            }

        // b
        if(ib < 0)
            if(v.distSq(b) < mergeDist) {
                ib = i/3;
            }

    }

    // Si on a pas pu fusionner, on le crée
    if(ia < 0){
        ia = vertices.size()/3;
        addVertex(a, na);

    }
    if(ib < 0){
        ib = vertices.size()/3;
        addVertex(b, nb);
    }

    // On push les indices
    indices.push_back(ia);
    indices.push_back(ib);
}

void Mesh::updateBuffers() {
    API3D::instance->updateMeshBuffers(*this);
}

void Mesh::append(Vec3 position, const Mesh& obj) {
    unsigned int indiceStart = vertices.size()/3;
    for(auto indice : obj.indices) {
        indices.push_back(indice + indiceStart);
    }
    for(int i = 0; i < obj.vertices.size(); i++) {
        vertices.push_back(obj.vertices[i] + position[i%3]);
        normales.push_back(obj.normales[i]);
        colors.push_back(obj.colors[i]);
    }
}

const std::vector<unsigned int> &Mesh::getIndices() const {
    return indices;
}

unsigned int Mesh::getVao() const {
    return vao;
}

void Mesh::clear() {
    vertices.clear();
    colors.clear();
    normales.clear();
    texcoords.clear();
    indices.clear();
}

Mesh::TriIndices Mesh::addTri(Vec3 a, Vec3 b, Vec3 c, Vec3 na, Vec3 nb, Vec3 nc, Vec3 txa, Vec3 txb, Vec3 txc, float mergeDist) {

    // Les indices des vertex dans le tableau "vertices"
    int ia = -1, ib = -1, ic = -1;
    mergeDist *= mergeDist;

    // On vérifie qu'on peux fusionner ou non des vertices
    // On check les 100 derniers (pas oublier de multiplier de 3 mdr)
    int checkLast = 100*3;
    // L'indice auquel un démarre
    int starti = std::max(0ull, (long long unsigned int)vertices.size()-checkLast);
    starti = 0;

    // Pour chaque vertex à vérifier
    for(int i = starti; i < vertices.size(); i+=3) {
        Vec3 v = {vertices[i], vertices[i+1], vertices[i+2]};

        // a
        if(ia < 0)
            if(v.distSq(a) < mergeDist) {
                ia = i/3;
            }

        // b
        if(ib < 0)
            if(v.distSq(b) < mergeDist) {
                ib = i/3;
            }

        // c
        if(ic < 0)
            if(v.distSq(c) < mergeDist) {
                ic = i/3;
            }

    }

    // Si on a pas pu fusionner, on le crée
    if(ia < 0){
        ia = vertices.size()/3;
        addVertex(a, na, txa);

    }
    if(ib < 0){
        ib = vertices.size()/3;
        addVertex(b, nb, txb);
    }
    if(ic < 0){
        ic = vertices.size()/3;
        addVertex(c, nc, txc);
    }

    // On push les indices
    // Premier triangle
    indices.push_back(ia);
    indices.push_back(ib);
    indices.push_back(ic);

    return {ia, ib, ic};
}

void Mesh::renderGUI() {

    ImGui::PushID(this);

    ImGui::Text("Name : %s", name.c_str());

    if (ImGui::BeginCombo("Albedo", TextureManager::getName(material.albedoMap).c_str())) {

        for (auto[texID, name] : TextureManager::getReverseForNames()) {
            bool clicked = material.albedoMap == texID;
            ImGui::Selectable(name.c_str(), &clicked);
            if (clicked && material.albedoMap != texID) material.albedoMap = texID;
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Roughness", TextureManager::getName(material.roughnessMap).c_str())) {

        for (auto[texID, name] : TextureManager::getReverseForNames()) {
            bool clicked = material.roughnessMap == texID;
            ImGui::Selectable(name.c_str(), &clicked);
            if (clicked && material.roughnessMap != texID) material.roughnessMap = texID;
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Normal", TextureManager::getName(material.normalMap).c_str())) {

        for (auto[texID, name] : TextureManager::getReverseForNames()) {
            bool clicked = material.normalMap == texID;
            ImGui::Selectable(name.c_str(), &clicked);
            if (clicked && material.normalMap != texID) material.normalMap = texID;
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Metallic", TextureManager::getName(material.metallicMap).c_str())) {

        for (auto[texID, name] : TextureManager::getReverseForNames()) {
            bool clicked = material.metallicMap == texID;
            ImGui::Selectable(name.c_str(), &clicked);
            if (clicked && material.metallicMap != texID) material.metallicMap = texID;
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("AO", TextureManager::getName(material.aoMap).c_str())) {

        for (auto[texID, name] : TextureManager::getReverseForNames()) {
            bool clicked = material.aoMap == texID;
            ImGui::Selectable(name.c_str(), &clicked);
            if (clicked && material.aoMap != texID) material.aoMap = texID;
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Emission", TextureManager::getName(material.emissionMap).c_str())) {

        for (auto[texID, name] : TextureManager::getReverseForNames()) {
            bool clicked = material.emissionMap == texID;
            ImGui::Selectable(name.c_str(), &clicked);
            if (clicked && material.emissionMap != texID) material.emissionMap = texID;
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Use lighting", &useLight);

    ImGui::PopID();
}

nlohmann::json Mesh::toJson() {
    nlohmann::json json;

    json["name"] = name;
    json["color"] = {color.x, color.y, color.z};
    json["material"] = material.toJson();
    json["drawingMethod"] = drawingMethod;
    json["useLight"] = useLight;

    return json;
}

Mesh::~Mesh() {}

void Mesh::parseJson(nlohmann::json json) {
    name = json["name"];
    color.x = json["color"][0];
    color.y = json["color"][1];
    color.z = json["color"][2];
    material = Material::fromJson(json["material"]);
    drawingMethod = json["drawingMethod"];
    useLight = json["useLight"];
}