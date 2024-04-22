#pragma once
#include "InteractorKeyboard.h"
#include "SystemManager.h"

InteractorKeyboard::InteractorKeyboard(SystemManager* systemManager) : m_systemManager(systemManager)
{
	// Setup callbacks from GLFW window
	glfwSetKeyCallback(systemManager->getRenderWindow(), &InteractorKeyboard::keyCallback);
	glfwSetInputMode(systemManager->getRenderWindow(), GLFW_LOCK_KEY_MODS, GLFW_TRUE);

}

void InteractorKeyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		globjects::debug() << "Insert spacebar interaction functionality here.";
	}

	// To add keys, just copy-paste the spacebar function above and change the key (GLFW_KEY_XXX).
	// GLFW_RELEASE is usually less prone to unintended double-presses. 
}