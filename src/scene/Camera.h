#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SHIFT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera 
{
public:
	Camera(const glm::vec3& position, const glm::vec3& up = glm::vec3(0, 1, 0), float yaw = YAW, float pitch = PITCH);

	glm::mat4 GetViewMatrix() const;

	void ProcessKeyboard(CameraMovement direction, float delta_time);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
	void ProcessMouseScroll(float yoffset);

	float Zoom() const;
	glm::vec3 Position() const;
	glm::vec3 Front() const;
	float Speed() const;

	void SetPosition(const glm::vec3& position);
	void SetSpeed(float speed);

private:
	void UpdateCameraVectors();

	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_world_up;

	float m_yaw;
	float m_pitch;

	float m_movement_speed;
	float m_mouse_sensitivity;
	float m_zoom;
};
