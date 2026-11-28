#include "Renderer.h"

#include <QCoreApplication>
#include <QDir>

#include "../scene/Camera.h"
#include "../scene/Mesh.h"

#include <glm/glm.hpp>

namespace 
{
    static const int POINT_LIGHTS_COUNT = 4;
    static const int SPOT_LIGHTS_COUNT = 3;
}

void Renderer::Initialize()
{
    QDir app_dir(QCoreApplication::applicationDirPath());


	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_model_shader = std::make_unique<Shader>(app_dir.filePath("assets/shaders/model.vs").toStdString(), 
        app_dir.filePath("assets/shaders/model.fs").toStdString(), true);
    m_light_shader = std::make_unique<Shader>(app_dir.filePath("assets/shaders/light.vs").toStdString(),
        app_dir.filePath("assets/shaders/light.fs").toStdString(), true);

    std::vector<Vertex> cube_vertices = {
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

    std::vector<unsigned int> cube_indices = {
         0,  1,  2,   2,  3,  0,
         4,  5,  6,   6,  7,  4,
         8,  9, 10,  10, 11,  8,
        12, 13, 14,  14, 15, 12,
        16, 17, 18,  18, 19, 16,
        20, 21, 22,  22, 23, 20
    };

    std::vector<Vertex> pyramid_vertices = {
        // Front
        Vertex{{-0.5f, -0.5f,  0.5f}, { 0.0f, 0.447214f,  0.894427f}, {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f}, { 0.0f, 0.447214f,  0.894427f}, {1.0f, 0.0f}},
        Vertex{{ 0.0f,  0.5f,  0.0f}, { 0.0f, 0.447214f,  0.894427f}, {0.5f, 1.0f}},

        // Right
        Vertex{{ 0.5f, -0.5f,  0.5f}, { 0.894427f, 0.447214f, 0.0f}, {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f, -0.5f}, { 0.894427f, 0.447214f, 0.0f}, {1.0f, 0.0f}},
        Vertex{{ 0.0f,  0.5f,  0.0f}, { 0.894427f, 0.447214f, 0.0f}, {0.5f, 1.0f}},

        // Back
        Vertex{{ 0.5f, -0.5f, -0.5f}, { 0.0f, 0.447214f, -0.894427f}, {0.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f}, { 0.0f, 0.447214f, -0.894427f}, {1.0f, 0.0f}},
        Vertex{{ 0.0f,  0.5f,  0.0f}, { 0.0f, 0.447214f, -0.894427f}, {0.5f, 1.0f}},

        // Left
        Vertex{{-0.5f, -0.5f, -0.5f}, {-0.894427f, 0.447214f, 0.0f}, {0.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f}, {-0.894427f, 0.447214f, 0.0f}, {1.0f, 0.0f}},
        Vertex{{ 0.0f,  0.5f,  0.0f}, {-0.894427f, 0.447214f, 0.0f}, {0.5f, 1.0f}},

        // Bottom
        Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
    };

    std::vector<unsigned int> pyramid_indices = {
         0,  1,  2,
         3,  4,  5,
         6,  7,  8,
         9, 10, 11,
        12, 13, 14,
        14, 15, 12
    };

    std::vector<Vertex> octahedron_vertices = {
        // Top front right
        Vertex{{ 0.0f,  0.5f,  0.0f}, { 0.57735f,  0.57735f,  0.57735f}, {0.5f, 1.0f}},
        Vertex{{ 0.0f,  0.0f,  0.5f}, { 0.57735f,  0.57735f,  0.57735f}, {0.0f, 0.0f}},
        Vertex{{ 0.5f,  0.0f,  0.0f}, { 0.57735f,  0.57735f,  0.57735f}, {1.0f, 0.0f}},

        // Top back right
        Vertex{{ 0.0f,  0.5f,  0.0f}, { 0.57735f,  0.57735f, -0.57735f}, {0.5f, 1.0f}},
        Vertex{{ 0.5f,  0.0f,  0.0f}, { 0.57735f,  0.57735f, -0.57735f}, {0.0f, 0.0f}},
        Vertex{{ 0.0f,  0.0f, -0.5f}, { 0.57735f,  0.57735f, -0.57735f}, {1.0f, 0.0f}},

        // Top back left
        Vertex{{ 0.0f,  0.5f,  0.0f}, {-0.57735f,  0.57735f, -0.57735f}, {0.5f, 1.0f}},
        Vertex{{ 0.0f,  0.0f, -0.5f}, {-0.57735f,  0.57735f, -0.57735f}, {0.0f, 0.0f}},
        Vertex{{-0.5f,  0.0f,  0.0f}, {-0.57735f,  0.57735f, -0.57735f}, {1.0f, 0.0f}},

        // Top front left
        Vertex{{ 0.0f,  0.5f,  0.0f}, {-0.57735f,  0.57735f,  0.57735f}, {0.5f, 1.0f}},
        Vertex{{-0.5f,  0.0f,  0.0f}, {-0.57735f,  0.57735f,  0.57735f}, {0.0f, 0.0f}},
        Vertex{{ 0.0f,  0.0f,  0.5f}, {-0.57735f,  0.57735f,  0.57735f}, {1.0f, 0.0f}},

        // Bottom front right
        Vertex{{ 0.0f, -0.5f,  0.0f}, { 0.57735f, -0.57735f,  0.57735f}, {0.5f, 0.0f}},
        Vertex{{ 0.5f,  0.0f,  0.0f}, { 0.57735f, -0.57735f,  0.57735f}, {1.0f, 1.0f}},
        Vertex{{ 0.0f,  0.0f,  0.5f}, { 0.57735f, -0.57735f,  0.57735f}, {0.0f, 1.0f}},

        // Bottom back right
        Vertex{{ 0.0f, -0.5f,  0.0f}, { 0.57735f, -0.57735f, -0.57735f}, {0.5f, 0.0f}},
        Vertex{{ 0.0f,  0.0f, -0.5f}, { 0.57735f, -0.57735f, -0.57735f}, {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.0f,  0.0f}, { 0.57735f, -0.57735f, -0.57735f}, {0.0f, 1.0f}},

        // Bottom back left
        Vertex{{ 0.0f, -0.5f,  0.0f}, {-0.57735f, -0.57735f, -0.57735f}, {0.5f, 0.0f}},
        Vertex{{-0.5f,  0.0f,  0.0f}, {-0.57735f, -0.57735f, -0.57735f}, {1.0f, 1.0f}},
        Vertex{{ 0.0f,  0.0f, -0.5f}, {-0.57735f, -0.57735f, -0.57735f}, {0.0f, 1.0f}},

        // Bottom front left
        Vertex{{ 0.0f, -0.5f,  0.0f}, {-0.57735f, -0.57735f,  0.57735f}, {0.5f, 0.0f}},
        Vertex{{ 0.0f,  0.0f,  0.5f}, {-0.57735f, -0.57735f,  0.57735f}, {1.0f, 1.0f}},
        Vertex{{-0.5f,  0.0f,  0.0f}, {-0.57735f, -0.57735f,  0.57735f}, {0.0f, 1.0f}}
    };

    std::vector<unsigned int> octahedron_indices = {
         0,  1,  2,
         3,  4,  5,
         6,  7,  8,
         9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23
    };

    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/faceit_avatar.jpg").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));

    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/dark_wood_diffuse.png").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/dark_wood_specular.png").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/gold_diffuse.png").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/gold_specular.png").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/marble_diffuse.png").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));
    m_textures.push_back(std::make_shared<Texture>(app_dir.filePath("assets/textures/marble_specular.png").toStdString(), GL_REPEAT, GL_REPEAT,
        GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST));

    // TODO: create TextureManager
    Material dark_wood;
    dark_wood.diffuse_map = m_textures[1];
    dark_wood.specular_map = m_textures[2];
    dark_wood.shininess = 32.0f;
    m_materials.push_back(std::make_shared<Material>(dark_wood));

    Material gold;
    gold.diffuse_map = m_textures[3];
    gold.specular_map = m_textures[4];
    gold.shininess = 32.0f;
    m_materials.push_back(std::make_shared<Material>(gold));

    Material marble;
    marble.diffuse_map = m_textures[5];
    marble.specular_map = m_textures[6];
    marble.shininess = 32.0f;
    m_materials.push_back(std::make_shared<Material>(marble));

    m_models.push_back(Model(Mesh(cube_vertices, cube_indices, 0), m_materials[static_cast<int>(MaterialType::Gold)]));
    m_models.push_back(Model(Mesh(pyramid_vertices, pyramid_indices, 0), m_materials[static_cast<int>(MaterialType::Marble)]));
    m_models.push_back(Model(Mesh(octahedron_vertices, octahedron_indices, 0), m_materials[static_cast<int>(MaterialType::DarkWood)]));

    m_models.push_back(Model(app_dir.filePath("assets/models/scrap_robot.obj").toStdString(), dark_wood));
    m_models.push_back(Model(app_dir.filePath("assets/models/starfighter.obj").toStdString(), dark_wood));
    m_models.push_back(Model(app_dir.filePath("assets/models/treasure_chest.obj").toStdString(), dark_wood));

    glm::vec3 point_light_positions[] = {
        glm::vec3(3.0f,  3.0f,  3.0f),
        glm::vec3(-3.0f,  3.0f,  3.0f),
        glm::vec3(-3.0f,  3.0f, -3.0f),
        glm::vec3(3.0f,  3.0f, -3.0f)
    };

    glm::vec3 point_light_colors[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.3f, 0.3f),
        glm::vec3(0.3f, 1.0f, 0.3f),
        glm::vec3(0.3f, 0.3f, 1.0f)
    };

    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) 
    {
        PointLight light;

        light.position = point_light_positions[i];

        light.ambient = point_light_colors[i] * 0.02f;
        light.diffuse = point_light_colors[i] * 0.8f;
        light.specular = point_light_colors[i] * 1.0f;

        light.constant = 1.0f;
        light.linear = 0.09f;
        light.quadratic = 0.032f;

        m_point_lights.push_back(light);
    }

    glm::vec3 spot_light_positions[] = {
        glm::vec3(0.0f, 0.0f, 3.0f), // Camera flashlight, update every frame
        glm::vec3(-4.0f, 5.0f, 3.0f),
        glm::vec3(4.0f, 4.0f, -3.0f)
    };

    glm::vec3 target(0.0f);

    glm::vec3 spot_light_directions[] = {
        glm::vec3(0.0f, 0.0f, -1.0f), // Camera flashlight, update every frame
        glm::normalize(target - spot_light_positions[1]),
        glm::normalize(target - spot_light_positions[2])
    };

    glm::vec3 spot_light_colors[] = {
        glm::vec3(1.0f, 0.88f, 0.65f),
        glm::vec3(1.0f, 0.8f, 0.5f),
        glm::vec3(1.0f, 0.9f, 0.6f)
    };

    float spot_light_cutoffs[] = {
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(17.5f)),
        glm::cos(glm::radians(20.0f))
    };

    float spot_light_outer_cutoffs[] = {
        glm::cos(glm::radians(17.5f)),
        glm::cos(glm::radians(25.0f)),
        glm::cos(glm::radians(27.5f))
    };

    for (int i = 0; i < SPOT_LIGHTS_COUNT; ++i) 
    {
        SpotLight light;

        light.position = spot_light_positions[i];
        light.direction = spot_light_directions[i];

        light.ambient = spot_light_colors[i] * 0.02f;
        light.diffuse = spot_light_colors[i] * 0.8f;
        light.specular = spot_light_colors[i] * 1.0f;

        light.constant = 1.0f;
        light.linear = 0.09f;
        light.quadratic = 0.032f;

        light.cut_off = spot_light_cutoffs[i];
        light.outer_cut_off = spot_light_outer_cutoffs[i];

        m_spot_lights.push_back(light);
    }
}

void Renderer::Render(const Camera* camera, float aspect_ratio)
{
    // TODO: refactor the entire class hierarchy, with this information placed in a separate class
    glm::vec3 model_global_positions[] = {
        glm::vec3(0.0f),
        glm::vec3(2.0f, 0.5f, -2.0f),
        glm::vec3(-3.0f, 1.0f, 0.0f),
        glm::vec3(3.0f, 2.0f, -1.0f),
        glm::vec3(-4.0f, 0.5f, -4.0f),
        glm::vec3(1.0f, 1.5f, -6.0f)
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_model_shader->Use();

	glm::mat4 model(1.0f);

	glm::mat4 view(camera->GetViewMatrix());
	m_model_shader->SetMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom()), aspect_ratio, 0.1f, 100.0f);
	m_model_shader->SetMat4("projection", projection);

    m_model_shader->SetVec3("viewPos", camera->Position());

    m_model_shader->SetVec3("dirLight.ambient", m_dir_light.ambient);
    m_model_shader->SetVec3("dirLight.diffuse", m_dir_light.diffuse);
    m_model_shader->SetVec3("dirLight.specular", m_dir_light.specular);
    m_model_shader->SetVec3("dirLight.direction", m_dir_light.direction);

    m_model_shader->SetInt("PointLightsCount", POINT_LIGHTS_COUNT);
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) 
    {
        std::string number = std::to_string(i);

        m_model_shader->SetVec3(("PointLights[" + number + "].ambient").c_str(), m_point_lights[i].ambient);
        m_model_shader->SetVec3(("PointLights[" + number + "].diffuse").c_str(), m_point_lights[i].diffuse);
        m_model_shader->SetVec3(("PointLights[" + number + "].specular").c_str(), m_point_lights[i].specular);
        m_model_shader->SetVec3(("PointLights[" + number + "].position").c_str(), m_point_lights[i].position);
        m_model_shader->SetFloat(("PointLights[" + number + "].constant").c_str(), m_point_lights[i].constant);
        m_model_shader->SetFloat(("PointLights[" + number + "].linear").c_str(), m_point_lights[i].linear);
        m_model_shader->SetFloat(("PointLights[" + number + "].quadratic").c_str(), m_point_lights[i].quadratic);
    }

    m_model_shader->SetInt("SpotLightsCount", SPOT_LIGHTS_COUNT);
    m_spot_lights[0].direction = camera->Front();
    m_spot_lights[0].position = camera->Position();
    for (int i = 0; i < SPOT_LIGHTS_COUNT; ++i) 
    {
        std::string number = std::to_string(i);

        m_model_shader->SetVec3(("SpotLights[" + number + "].ambient").c_str(), m_spot_lights[i].ambient);
        m_model_shader->SetVec3(("SpotLights[" + number + "].diffuse").c_str(), m_spot_lights[i].diffuse);
        m_model_shader->SetVec3(("SpotLights[" + number + "].specular").c_str(), m_spot_lights[i].specular);
        m_model_shader->SetVec3(("SpotLights[" + number + "].position").c_str(), m_spot_lights[i].position);
        m_model_shader->SetVec3(("SpotLights[" + number + "].direction").c_str(), m_spot_lights[i].direction);
        m_model_shader->SetFloat(("SpotLights[" + number + "].constant").c_str(), m_spot_lights[i].constant);
        m_model_shader->SetFloat(("SpotLights[" + number + "].linear").c_str(), m_spot_lights[i].linear);
        m_model_shader->SetFloat(("SpotLights[" + number + "].quadratic").c_str(), m_spot_lights[i].quadratic);
        m_model_shader->SetFloat(("SpotLights[" + number + "].cut_off").c_str(), m_spot_lights[i].cut_off);
        m_model_shader->SetFloat(("SpotLights[" + number + "].outer_cut_off").c_str(), m_spot_lights[i].outer_cut_off);
    }

    for (int i = 0; i < m_models.size(); ++i) 
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, model_global_positions[i]);

        m_model_shader->SetMat4("model", model);

        for (const Mesh& mesh : m_models[i].GetMeshes()) 
        {
            BindMaterial(m_models[i].GetMaterial(mesh.GetMaterialIndex().value()));

            mesh.Draw();
        }
    }

    m_light_shader->Use();
    m_light_shader->SetMat4("view", view);
    m_light_shader->SetMat4("projection", projection);
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) 
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_point_lights[i].position);
        model = glm::scale(model, glm::vec3(0.25f));
        m_light_shader->SetMat4("model", model);
        m_light_shader->SetVec3("LightColor", m_point_lights[i].specular); // TODO: specular not always equal to 1.0

        for (const Mesh& mesh : m_models[0].GetMeshes())
        {
            mesh.Draw();
        }
    }
    for (int i = 1; i < SPOT_LIGHTS_COUNT; ++i) 
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_spot_lights[i].position);
        model = glm::scale(model, glm::vec3(0.25f));
        m_light_shader->SetMat4("model", model);
        m_light_shader->SetVec3("LightColor", m_spot_lights[i].specular); // TODO: specular not always equal to 1.0
        
        for (const Mesh& mesh : m_models[0].GetMeshes())
        {
            mesh.Draw();
        }
    }
}

void Renderer::Shutdown() 
{
    m_model_shader.reset();
    m_light_shader.reset();

    m_point_lights.clear();
    m_spot_lights.clear();
    m_textures.clear();
    m_materials.clear();
    m_models.clear();
}

void Renderer::BindMaterial(const Material& material)
{
        material.diffuse_map->Bind(0);
        m_model_shader->SetInt("material.diffuse_map", 0);

        material.specular_map->Bind(1);
        m_model_shader->SetInt("material.specular_map", 1);

        m_model_shader->SetFloat("material.shininess", material.shininess);
}
