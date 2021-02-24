//
// Created by etien on 05/11/2020.
//

#include <GL/gl3w.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "API/OpenGL/Shader_Opengl.h"

unsigned int Shader_Opengl::baseShaderID;
std::map<std::pair<const char*, const char*>, ShaderID> Shader_Opengl::alreadyLoaded;

void Shader_Opengl::init() {
    baseShaderID = loadShaders("resources/Shaders/OpenGL/base.vert", "resources/Shaders/OpenGL/base.frag");
}

unsigned int Shader_Opengl::baseShader() {
    return baseShaderID;
}

void Shader_Opengl::setBasePVM(const glm::mat4 &projection, const glm::mat4 view, const glm::mat4 model) {
    GLuint PMatrixID = glGetUniformLocation(baseShaderID, "P");
    GLuint VMatrixID = glGetUniformLocation(baseShaderID, "V");
    GLuint MMatrixID = glGetUniformLocation(baseShaderID, "M");
    glUniformMatrix4fv(PMatrixID, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(VMatrixID, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(MMatrixID, 1, GL_FALSE, &model[0][0]);
}

GLuint Shader_Opengl::loadShaders(const char *vertex_file_path, const char *fragment_file_path) {

    auto pair = std::make_pair(vertex_file_path, fragment_file_path);
    if(alreadyLoaded.find(pair) != alreadyLoaded.end())
        return alreadyLoaded[pair];



    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader_Opengl code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s.\n", vertex_file_path);
        return 0;
    }

    // Read the Fragment Shader_Opengl code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader_Opengl
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader_Opengl
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader_Opengl
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader_Opengl
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
//    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    alreadyLoaded[pair] = ProgramID;

    return ProgramID;
}

void Shader_Opengl::uniform3fv(ShaderID shader, const char *name, float *v, size_t count) {
    GLuint uID = glGetUniformLocation(shader, name);
    glUniform3fv(uID, count, v);
}

int Shader_Opengl::isLoaded(std::pair<const char *, const char *> paths) {
    if(alreadyLoaded.find(paths) != alreadyLoaded.end())
        return alreadyLoaded[paths];
    return -1;
}

void Shader_Opengl::uniform1fv(int shader, const char *name, float *v, size_t count) {
    GLuint uID = glGetUniformLocation(shader, name);
    glUniform1fv(uID, count, v);
}

void Shader_Opengl::uniform1iv(int shader, const char *name, int *i, size_t count) {
    GLuint uID = glGetUniformLocation(shader, name);
    glUniform1iv(uID, count, i);
}

ShaderID Shader_Opengl::Compute::loadShaders(const char *file_path) {

    auto pair = std::make_pair(file_path, file_path);
    if(alreadyLoaded.find(pair) != alreadyLoaded.end())
        return alreadyLoaded[pair];

    ShaderID computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    // Read the Vertex Shader_Opengl code from the file
    std::string ShaderCode;
    std::ifstream VertexShaderStream(file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        ShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s.\n", file_path);
        return 0;
    }


    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader_Opengl
    printf("Compiling shader : %s\n", file_path);
    char const * VertexSourcePointer = ShaderCode.c_str();
    glShaderSource(computeShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(computeShaderID);

    // Check Vertex Shader_Opengl
    glGetShaderiv(computeShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(computeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(computeShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    //    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, computeShaderID);
    glLinkProgram(ProgramID);
    glDeleteShader(computeShaderID);


    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    alreadyLoaded[pair] = ProgramID;

    return ProgramID;
}
