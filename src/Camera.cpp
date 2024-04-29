#pragma once
#include "Camera.h"
#include "SystemManager.h"

Camera::Camera(SystemManager* systemManager) : m_systemManager(systemManager)
{
	// Set default camera transform
	recalculateCamera();
}

void Camera::recalculateCamera()
{
	m_viewport_size_x = m_systemManager->getRenderWindowSize_x();
	m_viewport_size_y = m_systemManager->getRenderWindowSize_y();
	m_viewMatrix = createViewMatrix();
	m_projectionMatrix = createProjectionMatrix();
}

glm::mat4 Camera::getViewMatrix()
{
	return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

int Camera::getViewportSize_x()
{
	return m_viewport_size_x;
}

int Camera::getViewportSize_y()
{
	return m_viewport_size_y;
}

glm::mat4 Camera::createViewMatrix() 
{
	return glm::lookAt(m_cameraPos, m_center, m_cameraUp);
}

glm::mat4 Camera::createProjectionMatrix()
{
	return glm::perspective(45.0f, ((gl::GLfloat)m_viewport_size_x / (gl::GLfloat)m_viewport_size_y), m_nearZ, m_farZ);
}

void Camera::MouseCursorPosEvent(int xpos, int ypos)
{
	m_xCurrent = xpos;
	m_yCurrent = ypos;

	if (m_rotating)
	{
		if (m_xCurrent != m_xPrevious || m_yCurrent != m_yPrevious)
		{
			glm::vec3 va = arcballVector(m_xPrevious, m_yPrevious);
			glm::vec3 vb = arcballVector(m_xCurrent, m_yCurrent);

			if (va != vb)
			{
				float angle = glm::acos(std::max(-1.0f, std::min(1.0f, glm::dot(va, vb))));
				glm::vec3 axis = glm::cross(va, vb);

				glm::mat4 viewTransform = m_viewMatrix;
				glm::mat4 inverseViewTransform = glm::inverse(viewTransform);
				glm::vec4 transformedAxis = inverseViewTransform * glm::vec4(axis, 0.0);

				glm::mat4 newViewTransform = glm::rotate(viewTransform, angle, glm::vec3(transformedAxis));
				m_viewMatrix = newViewTransform;
			}
		}
	}
	if (m_panning)
	{
		if (m_xCurrent != m_xPrevious || m_yCurrent != m_yPrevious)
		{
			float aspect = float(m_viewport_size_x) / float(m_viewport_size_y);
			glm::vec2 va = glm::vec2(2.0f * float(m_xPrevious) / float(m_viewport_size_x) - 1.0f, -2.0f * float(m_yPrevious) / float(m_viewport_size_y) + 1.0f);
			glm::vec2 vb = glm::vec2(2.0f * float(m_xCurrent) / float(m_viewport_size_x) - 1.0f, -2.0f * float(m_yCurrent) / float(m_viewport_size_y) + 1.0f);
			glm::vec2 d = vb - va;

			glm::mat4 newViewTransform = glm::translate(glm::mat4(1.0), glm::vec3(aspect * d.x, d.y, 0.0f)) * m_viewMatrix;
			m_viewMatrix = newViewTransform;
		}
	}
	if (m_zooming)
	{
		if (m_xCurrent != m_xPrevious || m_yCurrent != m_yPrevious)
		{
			glm::vec2 va = glm::vec2(2.0f * float(m_xPrevious) / float(m_viewport_size_x) - 1.0f, -2.0f * float(m_yPrevious) / float(m_viewport_size_y) + 1.0f);
			glm::vec2 vb = glm::vec2(2.0f * float(m_xCurrent) / float(m_viewport_size_x) - 1.0f, -2.0f * float(m_yCurrent) / float(m_viewport_size_y) + 1.0f);
			glm::vec2 d = vb - va;

			float l = std::abs(d.x) > std::abs(d.y) ? d.x : d.y;
			float s = 1.0;

			if (l > 0.0f)
			{
				s += std::min(0.5f, glm::length(d));
			}
			else
			{
				s -= std::min(0.5f, glm::length(d));
			}

			glm::mat4 viewTransform = m_viewMatrix;
			glm::mat4 newViewTransform = glm::scale(viewTransform, glm::vec3(s, s, s));
			m_viewMatrix = newViewTransform;
		}
	}

	m_xPrevious = m_xCurrent;
	m_yPrevious = m_yCurrent;
}

void Camera::MouseButtonEvent(int button, bool action)
{
	switch (button) {
	case 0: // Left
		m_rotating = action;
		break;
	case 1: // Right
		m_panning = action;
		break;
	case 2: // Middle
		m_zooming = action;
		break;
	}
}

glm::vec3 Camera::arcballVector(double x, double y)
{
	glm::vec3 p = glm::vec3(2.0f * float(x) / float(m_viewport_size_x) - 1.0f, -2.0f * float(y) / float(m_viewport_size_y) + 1.0f, 0.0);

	float length2 = p.x * p.x + p.y * p.y;

	if (length2 < 1.0f)
		p.z = sqrt(1.0f - length2);
	else
		p = glm::normalize(p);

	return p;
}

void Camera::MouseScrollEvent(double xoffset, double yoffset)
{
	glm::mat4 viewTransform = m_viewMatrix;
	glm::mat4 newViewTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, (yoffset / 8.0))) * viewTransform;
	m_viewMatrix = newViewTransform;
}
