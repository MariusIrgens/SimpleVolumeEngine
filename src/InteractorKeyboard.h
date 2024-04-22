// Handles keyboard interaction through GLFW window callback

#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class SystemManager;

class InteractorKeyboard
{
public:
	InteractorKeyboard(SystemManager* systemManager);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	SystemManager* m_systemManager;

};