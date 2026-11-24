#include <glad/glad.h>
#include "OpenGLWidget.h"

#include <iostream>
#include <QOpenGLContext>
#include <algorithm>
#include <glm/glm.hpp>

OpenGLWidget::OpenGLWidget(float screen_width, float screen_height, QWidget* parent)
    : QOpenGLWidget(parent), m_screen_size(screen_width, screen_height)
{
    setFocusPolicy(Qt::ClickFocus);
    setMouseTracking(true);
}

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

    m_renderer = std::make_unique<Renderer>();

    m_renderer->Initialize();

    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

    m_timer.start();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
    m_screen_size.width = w;
    m_screen_size.height = h;
}

// clear framebuffer, use shaders, set up uniforms, call glDrawElements/....
void OpenGLWidget::paintGL()
{
    qint64 elapsed = m_timer.restart();
    m_delta_time = elapsed / 1000.0f;
    m_delta_time = std::min(m_delta_time, 0.1f);

    ProcessInput();

    m_renderer->Render(m_camera.get(), m_screen_size.width / m_screen_size.height);

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        clearFocus();
        event->accept();
        return;
    }

    if (!event->isAutoRepeat())
    {
        m_pressed_keys.insert(event->key());
    }

    event->accept();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event) 
{
    if (!event->isAutoRepeat()) 
    {
        m_pressed_keys.erase(event->key());
        m_camera->SetSpeed(m_camera->Speed() * 2.0f);
    }

    QOpenGLWidget::keyReleaseEvent(event);
}

void OpenGLWidget::ProcessInput() 
{
    if (m_pressed_keys.contains(Qt::Key_W)) 
    {
        m_camera->ProcessKeyboard(CameraMovement::FORWARD, m_delta_time);
    }
    if (m_pressed_keys.contains(Qt::Key_S))
    {
        m_camera->ProcessKeyboard(CameraMovement::BACKWARD, m_delta_time);
    }
    if (m_pressed_keys.contains(Qt::Key_A))
    {
        m_camera->ProcessKeyboard(CameraMovement::LEFT, m_delta_time);
    }
    if (m_pressed_keys.contains(Qt::Key_D))
    {
        m_camera->ProcessKeyboard(CameraMovement::RIGHT, m_delta_time);
    }
    if (m_pressed_keys.contains(Qt::Key_Shift))
    {
        m_camera->SetSpeed(m_camera->Speed() / 2.0f);
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!hasFocus() || !m_camera) 
    {

        return;
    }

    const QPoint center = mapToGlobal(rect().center());
    const QPoint position = QCursor::pos();

    if (position == center) 
    {
        return;
    }

    const float xoffset = position.x() - center.x();
    const float yoffset = center.y() - position.y();

    m_camera->ProcessMouseMovement(xoffset, yoffset);

    QCursor::setPos(center);

    event->accept();
}

//void OpenGLWidget::wheelEvent(QWheelEvent* event) 
//{
//
//
//    event->accept();
//}

void OpenGLWidget::focusOutEvent(QFocusEvent* event)
{
    m_pressed_keys.clear();

    unsetCursor();

    QOpenGLWidget::focusOutEvent(event);
}

void OpenGLWidget::focusInEvent(QFocusEvent* event)
{
    QOpenGLWidget::focusInEvent(event);

    setCursor(Qt::BlankCursor);

    QCursor::setPos(mapToGlobal(rect().center()));
}
