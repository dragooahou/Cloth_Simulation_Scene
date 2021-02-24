//
// Created by etien on 05/11/2020.
//

#ifndef PROJET_LSYSTEM_SHADER_OPENGL_H
#define PROJET_LSYSTEM_SHADER_OPENGL_H

#include <map>
#include <glm/glm.hpp>

typedef unsigned int ShaderID;

class Shader_Opengl {

    static std::map<std::pair<const char*, const char*>, ShaderID> alreadyLoaded;

    // shader de base
    static ShaderID baseShaderID;

    int isLoaded(std::pair<const char*, const char*> paths);

public:
    static void init();

    static ShaderID baseShader();

    static void setBasePVM(const glm::mat4 &projection, const glm::mat4 view, const glm::mat4 model);

    static ShaderID loadShaders(const char * vertex_file_path,const char * fragment_file_path);

    static void uniform3fv(ShaderID shader, const char *name, float *v, size_t count);

    static void uniform1fv(int shader, const char *name, float *f, size_t count);

    static void uniform1iv(int shader, const char *name, int *i, size_t count);

    struct Compute {

        static ShaderID loadShaders(const char * file_path);

    };
};




#endif //PROJET_LSYSTEM_SHADER_OPENGL_H
