#include "Renderer.h"

#include <QCoreApplication>
#include <QDir>

#include "../scene/Camera.h"
#include "../scene/Mesh.h"

#include <glm/glm.hpp>

void Renderer::Initialize() 
{
    QDir app_dir(QCoreApplication::applicationDirPath());


	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	m_model_shader = std::make_unique<Shader>(app_dir.filePath("assets/shaders/model.vs").toStdString(), 
        app_dir.filePath("assets/shaders/model.fs").toStdString(), true);

    std::vector<Vertex> vertices = {
        Vertex{ { 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f} },
        Vertex{ {-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f} },
        Vertex{ {-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f} },
        Vertex{ { 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f} },

        Vertex{ {-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f} },
        Vertex{ { 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f} },
        Vertex{ { 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f} },
        Vertex{ {-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f} },

        Vertex{ {-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f} },
        Vertex{ {-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f} },
        Vertex{ {-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f} },
        Vertex{ {-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f} },

        Vertex{ { 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f} },
        Vertex{ { 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f} },
        Vertex{ { 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f} },
        Vertex{ { 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f} },

        Vertex{ {-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f} },
        Vertex{ { 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f} },
        Vertex{ { 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f} },
        Vertex{ {-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f} },

        Vertex{ {-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f} },
        Vertex{ { 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f} },
        Vertex{ { 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f} },
        Vertex{ {-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f} }
    };

    std::vector<unsigned int> indices = {
         0,  1,  2,   2,  3,  0,
         4,  5,  6,   6,  7,  4,
         8,  9, 10,  10, 11,  8,
        12, 13, 14,  14, 15, 12,
        16, 17, 18,  18, 19, 16,
        20, 21, 22,  22, 23, 20
    };

    m_mesh = std::make_unique<Mesh>(vertices, indices);

    m_texture = std::make_unique<Texture>(app_dir.filePath("assets/textures/faceit_avatar.jpg").toStdString(), GL_REPEAT, GL_REPEAT, 
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);

    m_dir_light.direction = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Renderer::Render(const Camera* camera, float aspect_ratio) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_model_shader->Use();

	glm::mat4 model(1.0f);
	m_model_shader->SetMat4("model", model);

	glm::mat4 view(camera->GetViewMatrix());
	m_model_shader->SetMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom()), aspect_ratio, 0.1f, 100.0f);
	m_model_shader->SetMat4("projection", projection);

    m_model_shader->SetInt("texture0", 0);
    m_texture->Bind(0);

    m_model_shader->SetVec3("viewPos", camera->Position());

    m_model_shader->SetVec3("dirLight.ambient", m_dir_light.ambient);
    m_model_shader->SetVec3("dirLight.diffuse", m_dir_light.diffuse);
    m_model_shader->SetVec3("dirLight.specular", m_dir_light.specular);
    m_model_shader->SetVec3("dirLight.direction", m_dir_light.direction);

    m_mesh->Draw();
}

void Renderer::Shutdown() 
{
	m_model_shader.reset();
}
