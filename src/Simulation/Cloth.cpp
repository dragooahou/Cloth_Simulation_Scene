//
// Created by etien on 03/02/2021.
//

#include <iostream>
#include <GL/gl3w.h>
#include "API/OpenGL/API3D_Opengl.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderFunctions.h"
#include "Scene.hpp"
#include "API/OpenGL/Shader_Opengl.h"

#include "Simulation/Cloth.h"
#include "JobDispatcher.h"

std::set<Cloth::Wind*> Cloth::allwinds;

bool Cloth::useGPU = false;

const std::vector<std::string> Cloth::PositionAttach::typeNames  = {
        "GameObject", // path = go_name
        "Cloth"       // path = go_name / component_no / particle x / particle y
    };


bool in_frustum(const glm::mat4& M, const glm::vec3& p, const glm::vec3 bias = {0, 0, 0}) {
    glm::vec4 Pclip = M * glm::vec4(p, 1.);
    return abs(Pclip.x) < Pclip.w + bias.x &&
           abs(Pclip.y) < Pclip.w + bias.y &&
           abs(Pclip.z) < Pclip.w + bias.z;
}


float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


std::shared_ptr<Particle> Cloth::getParticle(int x, int y) {
    return particles[y*num_particles_width + x];
}


void Cloth::setParticle(int x, int y, std::shared_ptr<Particle> p) {
    particles[y*num_particles_width + x] = p;
}

void Cloth::makeConstraint(int x1, int y1, int x2, int y2) {
    constraints.emplace_back(getParticlePtr(x1, y1), getParticlePtr(x2, y2));
    constraints.back().p1_pos = {x1, y1, 0};
    constraints.back().p2_pos = {x2, y2, 0};
}

Cloth::Cloth(float width, float height, int num_particles_width, int num_particles_height) :
        Component("Cloth"),
        num_particles_width(num_particles_width),
        gui_npw(num_particles_width),
        num_particles_height(num_particles_height),
        gui_nph(num_particles_height),
        width(width),
        gui_w(width),
        height(height),
        gui_h(height) {

        createParticles();

    makeMovable(0 , 0, false);
    makeMovable(num_particles_width - 1, 0, false);
}

void Cloth::createParticles() {
    particles.clear();
    constraints.clear();
    nonmovable.clear();
    attachedPosition.clear();
    constraintsForGLSL.clear();

    if(type == 1) {
        num_particles_width = 1;
    }

    //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles
    particles.resize(num_particles_width*num_particles_height);

    // creating particles in a grid of particles from (0,0,0) to (width,-height,0)
    for(int x=0; x<num_particles_width; x++) {
        for(int y=0; y<num_particles_height; y++) {

            Vec3 pos = Vec3(width * (x/(float)num_particles_width),
                            -height * (y/(float)num_particles_height),
                            0);
            pos.rotate(Vec3::UP, glm::radians(angle));
            particles[y*num_particles_width+x] = std::make_shared<Particle>(Particle(pos)); // insert particle in column x at y'th row

        }
    }

    // Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
    for(int x=0; x<num_particles_width; x++) {
        for(int y=0; y<num_particles_height; y++) {
            if (x<num_particles_width-1) makeConstraint(x,y,x+1,y);
            if (y<num_particles_height-1) makeConstraint(x,y,x,y+1);
            if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(x,y,x+1,y+1);
            if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(x+1,y, x,y+1);
        }
    }


    // Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
    for(int x=0; x<num_particles_width; x++) {
        for(int y=0; y<num_particles_height; y++) {
            if (x<num_particles_width-2) makeConstraint(x,y,x+2,y);
            if (y<num_particles_height-2) makeConstraint(x,y,x,y+2);
            if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(x,y,x+2,y+2);
            if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(x+2,y,x,y+2);
        }
    }

    // Pour le compute shader
    constraintsForGLSL.resize(constraints.size());
    {
        for (int i = 0; i < constraints.size(); i++) {
            constraintsForGLSL[i] = {
                    constraints[i].rest_distance,
                    constraints[i].p1_pos,
                    constraints[i].p2_pos
            };
        }

    }

    if(bufferConstraints == 0)
        glGenBuffers(1, &bufferConstraints);

    if(bufferParticles == 0)
        glGenBuffers(1, &bufferParticles);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferConstraints);
    glBufferData(GL_SHADER_STORAGE_BUFFER, constraintsForGLSL.size() * sizeof(Constraint::GLSLStruct), &constraintsForGLSL[0], GL_DYNAMIC_READ);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferParticles);
    glBufferData(GL_SHADER_STORAGE_BUFFER, num_particles_width * num_particles_height * sizeof(glm::vec3), &particlespos[0], GL_DYNAMIC_READ);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    computeshaderID = Shader_Opengl::Compute::loadShaders("resources/Shaders/OpenGL/cloth.compute.glsl");

    fillMesh();

}

Vec3 Cloth::calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3) {
    return Vec3::cross(p2->getPosition() - p1->getPosition(), p3->getPosition() - p1->getPosition());
}

void Cloth::addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction, float strenght) {
    Vec3 normal = calcTriangleNormal(p1,p2,p3);
    Vec3 d = normal.normalized();
    Vec3 force = normal*(d.dot(direction)) * strenght;
    p1->addForce(force);
    p2->addForce(force);
    p3->addForce(force);
}

void Cloth::windForce(const Vec3 direction, float strenght) {
    for(int x = 0; x<num_particles_width-1; x++) {
        for(int y=0; y<num_particles_height-1; y++) {
            addWindForcesForTriangle(getParticle(x+1,y).get(),getParticle(x,y).get(),getParticle(x,y+1).get(),direction, strenght);
            addWindForcesForTriangle(getParticle(x+1,y+1).get(),getParticle(x+1,y).get(),getParticle(x,y+1).get(),direction, strenght);
        }
    }
}

void Cloth::timeStep() {

    /*{

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferParticles);
        auto* ptr = static_cast<glm::vec3 *>( glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY) );

        for (int x = 0; x < num_particles_width; x++) {
            for (int y = 0; y < num_particles_height; y++) {
                ptr[x + num_particles_width * y] = getParticle(x, y)->getPosition();
            }
        }

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    API3D::instance->uniform1iv(computeshaderID, "width", &num_particles_width, 1);

    glUseProgram(computeshaderID);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferConstraints);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bufferParticles);

    for(int i=0; i<CONSTRAINT_ITERATIONS; i++) {// iterate over all constraints several times
        glDispatchCompute(1, constraints.size(), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    glUseProgram(0);



    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferParticles);
    auto* ptr = static_cast<glm::vec3 *>( glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY) );

    for(int x = 0; x < num_particles_width; x++) {
        for (int y = 0; y < num_particles_height; y++) {
            auto p = getParticle(x, y);
            if(p->movable)
                p->position = ptr[x + num_particles_width * y];
        }
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
*/

    for(int i=0; i<CONSTRAINT_ITERATIONS; i++) {// iterate over all constraints several times

        for(int j = 0; j < constraints.size(); j++) {
            constraints[j].satisfyConstraint(); // satisfy constraint.
        }

    }



    for(auto& particle : particles) {
        particle->timeStep(); // calculate the position of each particle at the next time step.
    }


    // reset normals (which where written to last frame)
    for(auto& particle : particles) {
        particle->resetNormal();
    }


    //create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
    for(int x = 0; x<num_particles_width-1; x++) {
        for(int y=0; y<num_particles_height-1; y++) {
            Vec3 normal = calcTriangleNormal(getParticle(x+1,y).get(),getParticle(x,y).get(),getParticle(x,y+1).get());
            getParticle(x+1,y)->addToNormal(normal);
            getParticle(x,y)->addToNormal(normal);
            getParticle(x,y+1)->addToNormal(normal);

            normal = calcTriangleNormal(getParticle(x+1,y+1).get(),getParticle(x+1,y).get(),getParticle(x,y+1).get());
            getParticle(x+1,y+1)->addToNormal(normal);
            getParticle(x+1,y)->addToNormal(normal);
            getParticle(x,y+1)->addToNormal(normal);
        }
    }

}

void Cloth::ballCollision(const Vec3 center, const float radius) {

    float radius_offset = radius + 0.001;

    for(auto& particle : particles) {

        Vec3 v = particle->getPosition() - center + gameObject->transform.getPosition();
        float l = v.mag();
        if (l < radius_offset) {  // if the particle is inside the ball
            particle->offsetPos(v.normalized() * (radius_offset-l)); // project the particle to the surface of the ball
        }

    }

}


void Cloth::floorCollision(float height) {

    for(auto& particle : particles) {

        float v = particle->getPosition().y - (height+0.01f);
        if (v < 0) {
            particle->offsetPos({0, -v, 0});
        }

    }

}

void Cloth::addForce(const Vec3 direction) {
    for(auto& particle : particles) {
        particle->addForce(direction); // add the forces to each particle
    }

}

void Cloth::fillClothMesh() {
    // Remplissage du mesh
    mesh.clear();
    mesh.setColor(1, 1, 1);

    particlesindices.resize(num_particles_width);
    for(auto& v : particlesindices) v.resize(num_particles_height, -1);

    particlespos.resize(num_particles_width * num_particles_height);

    float texZoom = 0.1;
    for(int x=0; x<num_particles_width-1; x++) {
        for (int y = 0; y < num_particles_height-1; y++) {
//            mesh.setColor(map(x, 0, num_particles_width, 0, 1), map(y, 0, num_particles_height, 0, 1), 0);
            auto indices1 = mesh.addTri(getParticle(x, y)->getPosition(),
                         getParticle(x, y+1)->getPosition(),
                         getParticle(x+1, y)->getPosition(),

                         getParticle(x, y)->accumulated_normal,
                         getParticle(x, y+1)->accumulated_normal,
                         getParticle(x+1, y)->accumulated_normal,

                        Vec3(x, y) * texZoom,
                        Vec3(x, y+1) * texZoom,
                        Vec3(x+1, y) * texZoom
                         );

            auto indices2 = mesh.addTri(getParticle(x+1, y+1)->getPosition(),
                        getParticle(x+1, y)->getPosition(),
                        getParticle(x, y+1)->getPosition(),

                        getParticle(x+1, y+1)->accumulated_normal,
                        getParticle(x+1, y)->accumulated_normal,
                        getParticle(x, y+1)->accumulated_normal,

                        Vec3(x+1, y+1) * texZoom,
                        Vec3(x+1, y) * texZoom,
                        Vec3(x, y+1) * texZoom
                        );

            particlesindices[x][y] = indices1.ia;
            particlesindices[x][y+1] = indices1.ib;
            particlesindices[x+1][y] = indices1.ic;

            particlesindices[x+1][y+1] = indices2.ia;
            particlesindices[x+1][y] = indices2.ib;
            particlesindices[x][y+1] = indices2.ic;
        }
    }

//    mesh.addVertex({0, 0, 0}, {0, 0, 0}, {0, 0, 0});

    for(int x=0; x<num_particles_width-1; x++) {
        for (int y = 0; y < num_particles_height-1; y++) {

            mesh.indices.push_back(particlesindices[x][y]);
            mesh.indices.push_back(particlesindices[x+1][y]);
            mesh.indices.push_back(particlesindices[x][y+1]);

            mesh.indices.push_back(particlesindices[x+1][y+1]);
            mesh.indices.push_back(particlesindices[x][y+1]);
            mesh.indices.push_back(particlesindices[x+1][y]);

        }
    }

//    std::cout << "\n == == == clothVertexIndices" << std::endl;
//    for(auto [pair, indice] : clothVertexIndices) {
//        std::cout << pair.first << " " << pair.second << " -> " << indice << std::endl;
//    }

//    std::cout << "nbr de particles : " << mesh.vertices.size()/3 << std::endl;

}


void Cloth::fillRopeMesh() {
    // Remplissage du mesh
    mesh.clear();
    mesh.setColor(1, 1, 1);

    for(int i = 0; i < num_particles_height-1; i++) {
        solidCylinder(&mesh, getParticle(0, i)->getPosition(), getParticle(0, i+1)->getPosition(), 0.04, 0.04, 6);
    }


    mesh.updateBuffers();

}

void Cloth::makeMovable(int x, int y, bool movable) {
    if(movable)
        nonmovable.erase(std::make_pair(x, y));
    else
        nonmovable.insert(std::make_pair(x, y));
    getParticle(x, y)->makeMovable(movable);
    getParticle(x, y)->getAttachedPosition = [this]() {
        if(gameObject == nullptr) return Vec3{0, 0, 0};
        return gameObject->transform.getPosition();
    };

}

void Cloth::update() {
    Component::update();

    if(in_camera_view && !beingUpdated) {

        beingUpdated = true;
        JobDispatcher::addJob([this] {


            addForce(Vec3(0, -0.005, 0)); // add gravity each frame, pointing down

//        windForce(Vec3(1,0,0.02), 1); // generate some wind each frame
            for (const auto &w : allwinds)
                w->apply(*this);


            timeStep(); // calculate the particle positions of the next frame


            // Collisions ici
            floorCollision(0);

//        cloth->ballCollision(m_gameObjects.front()->transform.getPosition(), SPHERE_RADIUS); // resolve collision with the ball

            updateVertexPositionBuffer();

            beingUpdated = false;

        });
    }
}

void Cloth::render(RenderingChain *renderingChain) {
    Component::render(renderingChain);

    renderingChain->addStep([this](Camera* camera) {

        if(type != 1) // If it is not a rope
            in_camera_view = in_frustum(
                    camera->projectionMat *camera->viewMat,
                    gameObject->transform.getPosition(),
                    {width * camera->aspect, height * camera->aspect, 0}
                    );

    });

    if(in_camera_view) {
        mesh.updateBuffers();
        mesh.render(renderingChain, &cloth_transform);
    }

}

void Cloth::renderGUI() {
    Component::renderGUI();

    {

        if(ImGui::BeginCombo("Type", typeNames[type].c_str())) {
            for(int i = 0; i < typeNames.size(); i++) {
                ImGui::PushID(i);
                if(ImGui::Selectable(typeNames[i].c_str(), type == i)) {
                    type = i;
                    createParticles();
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }

        ImGui::InputInt("Contraint strenght", &CONSTRAINT_ITERATIONS, 1, 1);
        ImGui::InputInt("Number particles width", &gui_npw);
        ImGui::InputInt("Number particles height", &gui_nph);
        ImGui::InputFloat("Width", &gui_w);
        ImGui::InputFloat("Height", &gui_h);
        ImGui::InputFloat("Angle", &angle);
        if(ImGui::Button("Regenerate mesh")) {
            num_particles_width = gui_npw;
            num_particles_height = gui_nph;
            width = gui_w;
            height = gui_h;
            createParticles();
        }

    }

    if(ImGui::CollapsingHeader("Fixed particles")) {

        for(auto [x, y] : nonmovable) {
            ImGui::PushID(x); ImGui::PushID(y);

            ImGui::Text("%i,%i", x, y);
            ImGui::SameLine();
            if(ImGui::Button("Set movable"))
                makeMovable(x, y, true);

            ImGui::SameLine();

            ImGui::InputFloat3("Position", &getParticle(x, y)->position[0]);

            ImGui::PopID(); ImGui::PopID();
        }
        ImGui::InputInt2("", &gui_v[0]);
        ImGui::SameLine();
        if(ImGui::Button("Make unmovable"))
            if(gui_v.x >= 0 && gui_v.x < num_particles_width && gui_v.y >= 0 && gui_v.y < num_particles_height)
                makeMovable(gui_v.x, gui_v.y, false);
    }

    if(ImGui::CollapsingHeader("Attached particles")) {

        for(auto& [particle, attach] : attachedPosition) {
            ImGui::PushID(&attach);

            ImGui::Text("%i,%i", particle.first, particle.second);
            ImGui::SameLine();
            ImGui::Text("| Path : %s", attach.path.c_str());

            ImGui::SameLine();

            ImGui::Text("| Type : %s", attach.typeNames[attach.type].c_str());

            ImGui::SameLine();

            if(ImGui::Button("Remove")) {
                attachedPosition.erase(particle);
                auto old = getParticle(particle.first, particle.second);
                old->movable = true;
                setParticle(particle.first, particle.second, std::make_shared<Particle>(*old));
            }

            ImGui::PopID();

        }

        {

            ImGui::PushID("add_attach_form");
            ImGui::Separator();

            char buf[256];
            strcpy(buf,  gui_attach_path.c_str());
            ImGui::InputText("Path", &buf[0], 256);
            gui_attach_path = buf;

            ImGui::InputFloat2("Particle", &gui_attach_coords[0]);
            if(ImGui::BeginCombo("Type", PositionAttach::typeNames[gui_attach_type].c_str())) {
                for(int i = 0; i < PositionAttach::typeNames.size(); i++)
                    if(ImGui::Selectable(PositionAttach::typeNames[i].c_str(), gui_attach_type != i))
                        gui_attach_type = i;
                ImGui::EndCombo();
            }


            if(ImGui::Button("Add attach")) {
                auto at = PositionAttach{
                        .type = gui_attach_type,
                        .path = gui_attach_path
                };
                attachParticle(gui_attach_coords.x, gui_attach_coords.y, at);
            }

            ImGui::PopID();
        }


    }

    if(ImGui::CollapsingHeader("Mesh")) mesh.renderGUI();
}

nlohmann::json Cloth::toJson() {
    nlohmann::json json = Component::toJson();

    json["type"] = type;
    json["CONSTRAINT_ITERATIONS"] = CONSTRAINT_ITERATIONS;
    json["num_particles_width"] = num_particles_width;
    json["num_particles_height"] = num_particles_height;
    json["width"] = width;
    json["height"] = height;
    json["mesh"] = mesh.toJson();
    json["angle"] = angle;

    for(auto& nonm : nonmovable)
        json["nonmovable"].push_back(nonm);

    int index = 0;
    for(auto [position, attach] : attachedPosition) {

        json["attachedParticles"][index]["position"] = position;
        json["attachedParticles"][index]["type"] = attach.type;
        json["attachedParticles"][index]["path"] = attach.path;

        index++;
    }


    return json;
}

void Cloth::parseJson(nlohmann::json json) {
    Component::parseJson(json);

    type = json["type"];
    CONSTRAINT_ITERATIONS = json["CONSTRAINT_ITERATIONS"];
    num_particles_width = json["num_particles_width"];
    num_particles_height = json["num_particles_height"];
    width = json["width"];
    height = json["height"];
    angle = json["angle"];

    gui_h = height;
    gui_w = width;
    gui_nph = num_particles_height;
    gui_npw = num_particles_width;

    createParticles();

    mesh.parseJson(json["mesh"]);

    nonmovable.clear();
    for(auto& nonm : json["nonmovable"])
        makeMovable(nonm[0], nonm[1], false);

    attachedPosition.clear();
    for(auto attach : json["attachedParticles"]) {
        auto at = PositionAttach{
                .type = attach["type"],
                .path = attach["path"]
        };
        attachParticle(attach["position"][0], attach["position"][1], at);
    }



    fillMesh();
}

void Cloth::fillMesh(bool updateBuffers) {
    switch (type) {
        case 0:
            fillClothMesh();
            break;
        case 1:
            fillRopeMesh();
            break;

    }

    if(updateBuffers)
        mesh.updateBuffers();
}

void Cloth::attachParticle(int x, int y, Cloth::PositionAttach p) {

    // Parse path
    switch (p.type) {

        case 0:
            for(auto& go : Scene::last_instance->m_gameObjects) {
                if(go->name == p.path) {
                    getParticle(x, y)->getAttachedPosition = [&go]() { return go->transform.getPosition(); };
                    getParticle(x, y)->movable = false;
                    getParticle(x, y)->position = Vec3{0, 0, 0};
                }
            }
            break;

        case 1:
            const char sep = '/';
            std::string firstword;
            std::string secondword;
            std::string thirdword;
            std::string fourthword;
            int i = 0;

            // Premier mot
            for(; p.path[i] != sep && i < p.path.size(); i++)
                firstword.push_back(p.path[i]);

            // Deuxième mot
            for(i++;  p.path[i] != sep && i < p.path.size(); i++)
                secondword.push_back(p.path[i]);

            // Troisième mot
            for(i++;  p.path[i] != sep && i < p.path.size(); i++)
                thirdword.push_back(p.path[i]);

            // Quatrième mot
            for(i++;  p.path[i] != '\0' && i < p.path.size(); i++)
                fourthword.push_back(p.path[i]);

            int compno;
            int xpos;
            int ypos;

            try {

                compno = std::stoi(secondword);
                xpos = std::stoi(thirdword);
                ypos = std::stoi(fourthword);

                // Parsage terminé
                for(auto& go : Scene::last_instance->m_gameObjects)
                    if(go->name == firstword) {
                        auto particle = ((Cloth*)go->getComponents().at(compno).get())->getParticle(xpos, ypos);
                        setParticle(x, y, particle);
                    }

            } catch(std::exception& e) {
                std::cerr << "Invalid path : " << firstword << "->" << secondword << "->" << thirdword << "->" << fourthword << std::endl;
                std::cerr << "Exception : " << e.what() << std::endl;
                return;
            }

            break;
    }

    attachedPosition[std::make_pair(x, y)] = p;

}

std::shared_ptr<Particle> *Cloth::getParticlePtr(int x, int y) {
    return &particles[y*num_particles_width + x];
}

void Cloth::updateVertexPositionBuffer() {

    if(type == 1) return;

    Vec3* verts = reinterpret_cast<Vec3 *>(&mesh.vertices[0]);
    Vec3* norms = reinterpret_cast<Vec3 *>(&mesh.normales[0]);
    for(int x=0; x<num_particles_width; x++) {
        for (int y = 0; y < num_particles_height; y++) {
            verts[particlesindices[x][y]] = getParticle(x, y)->getPosition();
            norms[particlesindices[x][y]] = getParticle(x, y)->accumulated_normal;
        }
    }

}


Cloth::Wind::Wind() : Component("Wind") {
    allwinds.insert(this);
}

void Cloth::Wind::apply(Cloth& cloth) {

    switch (type) {

        // Point light
        case 0:

            for(int x = 0; x < cloth.num_particles_width-1 ; x++) {
                for(int y=0; y < cloth.num_particles_height-1; y++) {

                    float dist = cloth.getParticle(x, y)->getPosition().distSq(gameObject->transform.getPosition());
                    if(dist < radius*radius) {


                        Vec3 v = cloth.getParticle(x, y)->getPosition() - gameObject->transform.getPosition();
                        Vec3 direction = v.normalized();

                        cloth.addWindForcesForTriangle(
                                cloth.getParticle(x+1,y).get(),
                                cloth.getParticle(x,y).get(),
                                cloth.getParticle(x,y+1).get(),
                                direction,
                                strenght * std::min((radius/dist), 10.0f));
                        cloth.addWindForcesForTriangle(
                                cloth.getParticle(x+1,y+1).get(),
                                cloth.getParticle(x+1,y).get(),
                                cloth.getParticle(x,y+1).get(),
                                direction,
                                strenght * std::min((radius/dist), 10.0f));

                    }

                }
            }
            break;

        // Vortex
        case 1:
            for(int x = 0; x < cloth.num_particles_width-1 ; x++) {
                for (int y = 0; y < cloth.num_particles_height - 1; y++) {

                    float dist = cloth.getParticle(x, y)->getPosition().distSq(gameObject->transform.getPosition());
                    if(dist < radius*radius) {


                        Vec3 v = cloth.getParticle(x, y)->getPosition() - gameObject->transform.getPosition();
                        Vec3 direction = v.normalized();

                        direction = Vec3::cross(direction, Vec3::UP) + Vec3{0, 0.1, 0};

                        cloth.addWindForcesForTriangle(
                                cloth.getParticle(x+1,y).get(),
                                cloth.getParticle(x,y).get(),
                                cloth.getParticle(x,y+1).get(),
                                direction,
                                strenght * std::min((radius/dist), 10.0f));
                        cloth.addWindForcesForTriangle(
                                cloth.getParticle(x+1,y+1).get(),
                                cloth.getParticle(x+1,y).get(),
                                cloth.getParticle(x,y+1).get(),
                                direction,
                                strenght * std::min((radius/dist), 10.0f));

                    }

                }
            }

            break;
    }

}

Cloth::Wind::~Wind() {
    allwinds.erase(this);
}

void Cloth::Wind::renderGUI() {
    Component::renderGUI();

    if(ImGui::BeginCombo("Type", typenames[type].c_str())) {
        for(int i = 0; i < typenames.size(); i++) {
            bool selected = type == i;
            ImGui::Selectable(typenames[i].c_str(), &selected);
            if(selected && type != i) type = i;
        }
        ImGui::EndCombo();
    }

    ImGui::InputFloat("Radius", &radius);
    ImGui::InputFloat("Strenght", &strenght);
}

nlohmann::json Cloth::Wind::toJson() {
    auto json = Component::toJson();

    json["type"] = type;
    json["radius"] = radius;
    json["strenght"] = strenght;

    return json;
}

void Cloth::Wind::parseJson(nlohmann::json json) {
    Component::parseJson(json);

    type = json["type"];
    radius = json["radius"];
    strenght = json["strenght"];
}
