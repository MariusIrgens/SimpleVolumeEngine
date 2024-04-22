// Handles GUI
// Changes variables directly
// To add interaction, use the following scheme

// Example:
// ImGui::SliderFloat("March Distance", &m_systemManager->getRendererVolume()->marchDistance, 0.0001f, 0.01f, "%.5f");

// &m_systemManager->getRendererVolume()->marchDistance points directly to a variable in a different object.
// By first pointing to the SystemManager, you can reach any other object from there, as long as a pointer to the object is available from SystemManager.

// Documentation:
// https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

#pragma once
#include <glm/glm.hpp>

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM <glbinding/gl/gl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

class SystemManager;

class InteractorGUI
{
public:
	InteractorGUI(SystemManager* systemManager);
	void beginFrame();
	void endFrame();
	void renderUi();
	void drawMenu();

private:
	SystemManager* m_systemManager;
};