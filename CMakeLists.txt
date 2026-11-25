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
set(ASSIMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp")

set(ASSIMP_LIB_DIR
    "${ASSIMP_DIR}/lib/vc${MSVC_TOOLSET_VERSION}"
)

add_library(assimp_prebuilt STATIC IMPORTED)

set_target_properties(assimp_prebuilt PROPERTIES
    IMPORTED_LOCATION
        "${ASSIMP_LIB_DIR}/Debug/assimp-vc${MSVC_TOOLSET_VERSION}-mtd.lib"

    INTERFACE_INCLUDE_DIRECTORIES
        "${ASSIMP_DIR}/include"
)

# Zlib
add_library(zlib_prebuilt STATIC IMPORTED)

set_target_properties(zlib_prebuilt PROPERTIES
    IMPORTED_LOCATION
        "${ASSIMP_LIB_DIR}/Debug/zlibstaticd.lib"
)

# Assimp dependencies
target_link_libraries(assimp_prebuilt INTERFACE
    zlib_prebuilt
)

# stb image
add_library(stb_image STATIC Dependencies/stb/stb_image.cpp)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/stb)


add_executable(MeshView src/main.cpp src/gui/MainWindow.h src/gui/MainWindow.cpp src/gui/OpenGLWidget.h src/gui/OpenGLWidget.cpp
						src/renderer/Shader.h src/renderer/Shader.cpp src/renderer/Renderer.h src/renderer/Renderer.cpp src/scene/Camera.h 
						src/scene/Camera.cpp src/scene/Mesh.h src/scene/Mesh.cpp src/scene/Model.h src/scene/Model.cpp
						src/renderer/Texture.h src/renderer/Texture.cpp)

target_link_libraries(MeshView PRIVATE Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets glad glm assimp_prebuilt stb_image)

# Copy assets to executable directory
add_custom_command(TARGET MeshView POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:MeshView>/assets"
)
