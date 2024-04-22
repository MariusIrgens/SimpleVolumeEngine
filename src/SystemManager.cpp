#pragma once
#include "SystemManager.h"

void error_callback(int errnum, const char* errmsg)
{
	globjects::critical() << errnum << ": " << errmsg << std::endl;
}

SystemManager::SystemManager()
{
	// ***GLFW***
	// https://www.glfw.org/

	if (!glfwInit())
	{
		// TODO: ERROR HANDLING
	}

	// Setup GLFW with OpenGL
	glfwSetErrorCallback(error_callback);
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);

	// Create main window
	m_renderWindow = glfwCreateWindow(m_renderWindowSize_x, m_renderWindowSize_y, "Render window", NULL, NULL);
	if (m_renderWindow == nullptr)
	{
		globjects::critical() << "Context creation failed - terminating execution.";
		glfwTerminate();
	}

	makeRenderContextCurrent();

	// ***GLOBJECTS***
	// https://globjects.org/#program

	globjects::init([](const char* name) // Initialize globjects (internally initializes glbinding, and registers the current context)
		{
			return glfwGetProcAddress(name);
		});

	globjects::DebugMessage::enable(); 	// Enable debug logging

	// GLFW CALLBACK

	glfwSetWindowUserPointer(m_renderWindow, static_cast<void*>(this));
	glfwSetFramebufferSizeCallback(m_renderWindow, &framebufferSizeCallbackRenderWindow);

	// ***CREATE OBJECTS***

	m_scene = std::make_unique<Scene>(this);
	m_rendererVolume = std::make_unique<RendererVolume>(this);
	m_interactorKeyboard = std::make_unique<InteractorKeyboard>(this);
	m_interactorMouse = std::make_unique<InteractorMouse>(this);
	m_interactorGUI = std::make_unique<InteractorGUI>(this);
}

void SystemManager::frameloop()
{
	// This is the main rendering loop
	while (!glfwWindowShouldClose(m_renderWindow))
	{
		glfwPollEvents();					// Poll for interaction (keyboard and mouse)
		//m_scene->update();				// Use this function to update the scene
		m_interactorGUI->beginFrame();
		m_interactorGUI->drawMenu();
		m_rendererVolume->renderFrame();	// The framewise volume rendering is done in this function
		m_interactorGUI->endFrame();		// The GUI needs to render on top of volume render

		glfwSwapBuffers(m_renderWindow);
	}
	shutdown();
}

void SystemManager::shutdown()
{
	glfwDestroyWindow(m_renderWindow);
	glfwTerminate();
}

// TODO: Fix window resize callback (currently out-of-order)
void SystemManager::framebufferSizeCallbackRenderWindow(GLFWwindow* window, int width, int height)
{
	SystemManager* systemManager = static_cast<SystemManager*>(glfwGetWindowUserPointer(window));
	systemManager->setRenderWindowSize(width, height);
	systemManager->getScene()->getCamera()->setViewportSize();
	globjects::debug() << "resized window";
}

void SystemManager::setRenderWindowSize(int width, int height)
{
	m_renderWindowSize_x = width;
	m_renderWindowSize_y = height;
}

int SystemManager::getRenderWindowSize_x()
{
	return m_renderWindowSize_x;
}

int SystemManager::getRenderWindowSize_y()
{
	return m_renderWindowSize_y;
}

GLFWwindow* SystemManager::getRenderWindow()
{
	return m_renderWindow;
}

void SystemManager::makeRenderContextCurrent()
{
	glfwMakeContextCurrent(m_renderWindow);
}

Scene* SystemManager::getScene()
{
	return m_scene.get();
}

RendererVolume* SystemManager::getRendererVolume()
{
	return m_rendererVolume.get();
}
