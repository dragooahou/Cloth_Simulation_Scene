cmake_minimum_required(VERSION 3.16)
project(ClothSim)

set(CMAKE_CXX_STANDARD 17)

include_directories("include")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


find_package(GLM REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


file(GLOB Projet_LSystem_SRC
        "src/*.cpp"
        "src/Rendering/*.cpp"
        "src/ImGui/*.cpp"
        "src/GUI/*.cpp"
        "src/Simulation/*.cpp"
        "src/ImGui/impl/imgui_impl_glfw.cpp"
        )

file(GLOB Projet_LSystem_SRC_OPENGL
        "src/API/OpenGL/*.cpp"
        "src/API/OpenGL/*.c"
        )

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Zone resources
ADD_CUSTOM_TARGET(
        copy_resources_files

        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources

        COMMENT "Copying Files for target: Projet_LSystem" VERBATIM
)


add_executable(ClothSim ${Projet_LSystem_SRC} ${Projet_LSystem_SRC_OPENGL} src/ImGui/impl/imgui_impl_opengl3.cpp)

target_compile_definitions(ClothSim PUBLIC API3D_OPENGL)
target_include_directories(ClothSim PUBLIC ${OpenGL_INCLUDE_DIRS})


# Windows
if (WIN32)
    target_link_libraries(ClothSim ${OPENGL_gl_LIBRARY} glfw assimp)
endif()

# Linux
if (UNIX AND NOT APPLE)
    target_link_libraries(ClothSim ${OPENGL_gl_LIBRARY} glfw dl X11 pthread assimp)
endif ()

target_compile_options(ClothSim
        PRIVATE
        "-O3"
        )

ADD_DEPENDENCIES(ClothSim copy_resources_files)



