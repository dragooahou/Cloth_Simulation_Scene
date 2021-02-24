//
// Created by etien on 27/10/2020.
//

#ifndef PROJET_LSYSTEM_MESH_H
#define PROJET_LSYSTEM_MESH_H

#include <vector>
#include "Vec3.h"
#include "Material.h"
#include "RenderingChain.h"
#include "Transform.h"

class Mesh {

public:
    struct TriIndices {
        int ia;
        int ib;
        int ic;
    };

    std::vector<float> vertices, colors, normales, texcoords;
    std::vector<unsigned int> indices;

    Vec3 color = {1, 1, 1};

    // les différents buffers
    unsigned int vertexbuffer = 0;
    unsigned int colorbuffer = 0;
    unsigned int normalbuffer = 0;
    unsigned int texcoordsbuffer = 0;
    unsigned int vao = 0;

    Material material;

    bool useLight = true;

    std::string name;

    // Default : GL_TRIANGLES
    int drawingMethod = 4;

    Mesh();
    ~Mesh();

    void addLine(Vec3 a, Vec3 b, Vec3 na, Vec3 nb, float mergeDist = 0.0001f);
    TriIndices addTri(Vec3 a, Vec3 b, Vec3 c, Vec3 na, Vec3 nb, Vec3 nc, Vec3 txa, Vec3 txb, Vec3 txc, float mergeDist = 0.0001f);
    void addQuad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, float mergeDist = 0.0001f);

    void append(Vec3 position, const Mesh& obj);

    void setColor(float r, float g, float b);

    virtual void render(RenderingChain* renderChain, Transform* transform);

    void renderGUI();

    const std::vector<float> &getVertices() const;
    const std::vector<float> &getColors() const;

    size_t getVertexCount();

    void updateBuffers();

    const std::vector<unsigned int> &getIndices() const;

    unsigned int getVao() const;

    void clear();

    void addVertex(Vec3 v, Vec3 normal, Vec3 texCoord = {0, 0, 0});
    void addVertex(float x, float y, float z, float nx, float ny, float nz, float txx, float txy);

    nlohmann::json toJson();
    void parseJson(nlohmann::json json);


};


#endif //PROJET_LSYSTEM_MESH_H
