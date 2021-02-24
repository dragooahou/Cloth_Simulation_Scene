//
// Created by etien on 03/02/2021.
//

#ifndef CLOTHSIM_CLOTH_H
#define CLOTHSIM_CLOTH_H


#include <vector>
#include <Rendering/GameObject.h>
#include <set>
#include "Particle.h"
#include "Constraint.h"


class Cloth : public Component {

    std::vector<std::string> typeNames = {
            "Cloth",
            "Rope"
    };

    int type = 0;

    int CONSTRAINT_ITERATIONS = 10;

    std::vector<std::shared_ptr<Particle>> particles;
    std::vector<Constraint> constraints;
    int num_particles_width;
    int num_particles_height;
    float width;
    float height;

    std::set<std::pair<int, int>> nonmovable;

    struct PositionAttach {

        static const std::vector<std::string> typeNames;

        int type = 0;
        std::string path;
    };

    std::map<std::pair<int, int>, PositionAttach> attachedPosition;

    Mesh mesh;

    Transform cloth_transform;

    bool in_camera_view = true;

    // GUI
    int gui_npw = num_particles_width;
    int gui_nph = num_particles_height;
    float gui_w = width;
    float gui_h = height;
    glm::vec<2, int> gui_v;

    int gui_attach_type = 0;
    Vec3 gui_attach_coords;
    std::string gui_attach_path;

public:

    Cloth(float width, float height, int num_particles_width, int num_particles_height);

    void createParticles();

    std::shared_ptr<Particle> getParticle(int x, int y);
    std::shared_ptr<Particle>* getParticlePtr(int x, int y);
    void setParticle(int x, int y, std::shared_ptr<Particle> p);

    void makeConstraint(int x1, int y1, int x2, int y2);

    /* used to add gravity (or any other arbitrary vector) to all particles*/
    void addForce(Vec3 direction);

    /* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the
    triangle area as seen from the wind direction*/
    void windForce(Vec3 direction, float strenght);

    void timeStep();

    /* used to detect and resolve the collision of the cloth with the ball.
	This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
	This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
	*/
    void ballCollision(const Vec3 center,const float radius );
    void floorCollision(float height);

    void fillMesh(bool updateBuffers = true);
    void fillClothMesh();
    void fillRopeMesh();

    void makeMovable(int x, int y, bool movable);

    void update() override;

    void render(RenderingChain *renderingChain) override;

    void renderGUI() override;

    nlohmann::json toJson() override;
    void parseJson(nlohmann::json json) override;

    // region Wind

    struct Wind : public Component {
        Wind();
        ~Wind() override;

        std::vector<std::string> typenames = {
                "Point",
                "Vortex"
        };

        int type = 0;

        float radius = 1;
        float strenght = 1;

        void apply(Cloth& cloth);
        void renderGUI() override;
        nlohmann::json toJson() override;
        void parseJson(nlohmann::json json) override;
    };

    static std::set<Wind*> allwinds;
    // endregion

    static bool useGPU;
    unsigned int bufferConstraints = 0;
    unsigned int bufferParticles = 0;
    std::vector<Constraint::GLSLStruct> constraintsForGLSL;
    std::vector<std::vector<int>> particlesindices;
    std::vector<glm::vec3> particlespos;
    unsigned int computeshaderID;
    bool beingUpdated = false;


    void updateVertexPositionBuffer();

private:

    /* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the
	normal vector of the triangle defined by the position of the particles p1, p2, and p3.
	The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
	*/
    Vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);

    /* A private method used by windForce() to calcualte the wind force for a single triangle
    defined by p1,p2,p3*/
    void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction, float strenght);

    float angle = 0;

    void attachParticle(int x, int y, PositionAttach p);
};


#endif //CLOTHSIM_CLOTH_H
