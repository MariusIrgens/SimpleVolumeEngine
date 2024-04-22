#include "InteractorMouse.h"
#include "SystemManager.h"

InteractorMouse::InteractorMouse(SystemManager* systemManager) : m_systemManager(systemManager)
{
	glfwSetCursorPosCallback(systemManager->getRenderWindow(), &InteractorMouse::CursorPosCallback);
	glfwSetMouseButtonCallback(systemManager->getRenderWindow(), &InteractorMouse::MouseButtonCallback);
	glfwSetScrollCallback(systemManager->getRenderWindow(), &InteractorMouse::ScrollCallback);
}

void InteractorMouse::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	SystemManager* systemManager = static_cast<SystemManager*>(glfwGetWindowUserPointer(window));

	if (systemManager) 
	{
		ImGuiIO& io = ImGui::GetIO();
		// If ImGui is using mouse, do not interact with the 3D view
		if (io.WantCaptureMouse)
			return;

		systemManager->getScene()->getCamera()->MouseCursorPosEvent(xpos, ypos);
		// You can add functionality here
	}
}

void InteractorMouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	SystemManager* systemManager = static_cast<SystemManager*>(glfwGetWindowUserPointer(window));

	if (systemManager)
	{
		ImGuiIO& io = ImGui::GetIO();
		// If ImGui is using mouse, do not interact with the 3D view
		if (io.WantCaptureMouse)
			return;

		systemManager->getScene()->getCamera()->MouseButtonEvent(button, action);
		// You can add functionality here
	}
}

void InteractorMouse::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	SystemManager* systemManager = static_cast<SystemManager*>(glfwGetWindowUserPointer(window));

	if (systemManager)
	{
		systemManager->getScene()->getCamera()->MouseScrollEvent(xoffset, yoffset);
		// You can add functionality here
	}
}