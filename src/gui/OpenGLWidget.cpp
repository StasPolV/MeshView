#include <glad/glad.h>
#include "OpenGLWidget.h"

#include <iostream>
#include <QOpenGLContext>

// Creating VAO, VBO, EBO, Shaders, Textures, setting up OpenGL
void OpenGLWidget::initializeGL()
{
    if (!gladLoadGLLoader(
        [](const char* name) -> void*
        {
            return reinterpret_cast<void*>(
                QOpenGLContext::currentContext()
                ->getProcAddress(name)
                );
        }))
    {
        std::cerr << "Failed to initialize GLAD!\n";
        return;
    }

    glEnable(GL_DEPTH_TEST);

}

void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

// clear framebuffer, use shaders, set up uniforms, call glDrawElements/....
void OpenGLWidget::paintGL()
{

}
