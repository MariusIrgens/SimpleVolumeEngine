// Constructs all other objects
// Creates the main window
// Runs the frame loop

#pragma once
#include "Scene.h"
#include "RendererVolume.h"
#include "InteractorKeyboard.h"
#include "InteractorMouse.h"
#include "InteractorGUI.h"

#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/Meta.h>
#include <glbinding-aux/types_to_string.h>
#include <glbinding-aux/ValidVersions.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

class SystemManager
{
public:
	SystemManager();
	void frameloop();
	void makeRenderContextCurrent();
	GLFWwindow* getRenderWindow();
	Scene* getScene();
	RendererVolume* getRendererVolume();
	int getRenderWindowSize_x();
	int getRenderWindowSize_y();

private:
	void shutdown();
	static void framebufferSizeCallbackRenderWindow(GLFWwindow* window, int width, int height);
	void setRenderWindowSize(int width, int height);

	std::unique_ptr<Scene> m_scene;
	std::unique_ptr<RendererVolume> m_rendererVolume;
	std::unique_ptr<InteractorKeyboard> m_interactorKeyboard;
	std::unique_ptr<InteractorMouse> m_interactorMouse;
	std::unique_ptr<InteractorGUI> m_interactorGUI;
	GLFWwindow* m_renderWindow;
	int m_renderWindowSize_x = 1280;	// initial value - change if the window starts to big/small
	int m_renderWindowSize_y = 720;		// initial value - change if the window starts to big/small
};

// Code by Marius Irgens