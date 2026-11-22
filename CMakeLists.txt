cmake_minimum_required(VERSION 3.36)

project(MeshView LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

#GLAD
add_library(glad STATIC Dependencies/glad/src/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glad/include
)

#GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glm
)

# Assimp
set(ASSIMP_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp"
)

add_library(assimp_prebuilt STATIC IMPORTED)

set_target_properties(assimp_prebuilt PROPERTIES
    IMPORTED_LOCATION
        "${ASSIMP_DIR}/lib/Debug/assimp-vc145-mtd.lib"

    INTERFACE_INCLUDE_DIRECTORIES
        "${ASSIMP_DIR}/include"
)	
	

add_executable(MeshView src/main.cpp src/gui/MainWindow.h src/gui/MainWindow.cpp src/gui/OpenGLWidget.h src/gui/OpenGLWidget.cpp
						src/renderer/Shader.h src/renderer/Shader.cpp src/renderer/Renderer.h src/renderer/Renderer.cpp src/scene/Camera.h 
						src/scene/Camera.cpp src/scene/Mesh.h src/scene/Mesh.cpp src/scene/Model.h src/scene/Model.cpp)

target_link_libraries(MeshView PRIVATE Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets glad glm assimp_prebuilt)
