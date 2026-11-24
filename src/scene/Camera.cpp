#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
	: m_position(position), m_world_up(up), m_yaw(yaw), m_pitch(pitch), 
	  m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM)
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const 
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float delta_time) 
{
	float velocity = m_movement_speed * delta_time;
	if (direction == CameraMovement::FORWARD) 
	{
		m_position += m_front * velocity;
	}
	else if (direction == CameraMovement::BACKWARD) 
	{
		m_position -= m_front * velocity;
	}
	else if (direction == CameraMovement::RIGHT) 
	{
		m_position += m_right * velocity;
	}
	else if (direction == CameraMovement::LEFT)
	{
		m_position -= m_right * velocity;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch) 
{
	xoffset *= m_mouse_sensitivity;
	yoffset *= m_mouse_sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (constrain_pitch) 
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		} 

		if (m_pitch < -89.0f) 
		{
			m_pitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	m_zoom -= (float)yoffset;
	if (m_zoom < 1.0f) 
	{
		m_zoom = 1.0f;
	}
	if (m_zoom > 45.0f) 
	{
		m_zoom = 45.0f;
	}
}

float Camera::Zoom() const
{
	return m_zoom;
}

glm::vec3 Camera::Position() const
{
	return m_position;
}

glm::vec3 Camera::Front() const
{
	return m_front;
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

void Camera::UpdateCameraVectors() 
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = front;
	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
