// Handles mouse interaction through GLFW window callback

#pragma once

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


class SystemManager;

class InteractorMouse
{
public:
	InteractorMouse(SystemManager* systemManager);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	double xposition;
	double yposition;

private:
	SystemManager* m_systemManager;
};