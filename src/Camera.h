// Handles the View and Projection matrix
// Handles camera interaction

#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>

class SystemManager;

class Camera
{
public:
	Camera(SystemManager* systemManager);
	void recalculateCamera();
	glm::mat4 getViewMatrix();
	glm::mat4 createViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 createProjectionMatrix();
	int getViewportSize_x();
	int getViewportSize_y();
	void MouseCursorPosEvent(int xpos, int ypos);			// Called from InteractorMouse
	void MouseButtonEvent(int button, bool action);			// Called from InteractorMouse
	void MouseScrollEvent(double xoffset, double yoffset);	// Called from InteractorMouse

private:
	glm::vec3 arcballVector(double x, double y);			// Rotates camera

	SystemManager* m_systemManager;
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	int m_viewport_size_x;
	int m_viewport_size_y;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;

	// *** OPENGL MATHEMATICS ***
	// https://github.com/g-truc/glm/blob/master/manual.md

	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 m_center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	bool m_rotating = false;	// left mousebutton
	bool m_zooming = false;		// scroll wheel
	bool m_panning = false;		// right mousebutton
	double m_xPrevious = 0.0, m_yPrevious = 0.0;
	double m_xCurrent = 0.0, m_yCurrent = 0.0;
};

