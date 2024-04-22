#pragma once
#include "InteractorGUI.h"
#include "SystemManager.h"

InteractorGUI::InteractorGUI(SystemManager* systemManager) : m_systemManager(systemManager)
{
	// Setup ImGUI
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(systemManager->getRenderWindow(), true);
	ImGui_ImplOpenGL3_Init();
	io.Fonts->AddFontFromFileTTF("./../dat/Lato-Semibold.ttf", 18);
}

void InteractorGUI::drawMenu()
{
	 // ***TOP BAR***

		// FILE

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
			globjects::debug() << "Put save logic here";
		if (ImGui::MenuItem("Save As..."))
			globjects::debug() << "Put save as logic here";
		if (ImGui::MenuItem("Load"))
			globjects::debug() << "Put load logic here";
		if (ImGui::MenuItem("Exit"))
			glfwSetWindowShouldClose(m_systemManager->getRenderWindow(), GLFW_TRUE);

		ImGui::EndMenu();
	}

	// VOLUME

	if (ImGui::BeginMenu("Volume"))
	{
		ImGui::SliderFloat("March Distance", &m_systemManager->getRendererVolume()->marchDistance, 0.0001f, 0.01f, "%.5f");
		ImGui::SliderFloat("Density multiplier", &m_systemManager->getRendererVolume()->densityMultiplier, 0.01f, 1.0f, "%.5f");

		float TFMaxValue = m_systemManager->getScene()->getVolume()->getBiggestDataValue();
		ImGui::SliderFloat("Transfer Function Mid Value", &m_systemManager->getRendererVolume()->transferFunctionMidValue, 0.0f, TFMaxValue, "%.5f");
		ImGui::SliderFloat("Transfer Function Value Range", &m_systemManager->getRendererVolume()->transferFunctionValueRange, 0.0f, TFMaxValue/2, "%.5f");

		ImGui::EndMenu();
	}
}


void InteractorGUI::beginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	// Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
	ImGui::NewFrame();
	ImGui::BeginMainMenuBar();
}

void InteractorGUI::endFrame()
{
	// FPS INFORMATION

	static std::list<float> frameratesList;
	frameratesList.push_back(ImGui::GetIO().Framerate);

	while (frameratesList.size() > 64)
		frameratesList.pop_front();

	static float framerates[64];
	int i = 0;
	for (auto v : frameratesList)
		framerates[i++] = v;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << ImGui::GetIO().Framerate << " fps";
	std::string s = stream.str();

	ImGui::SameLine(ImGui::GetWindowWidth() - 220.0f);
	ImGui::PlotLines(s.c_str(), framerates, int(frameratesList.size()), 0, 0, 0.0f, 200.0f, ImVec2(128.0f, 0.0f));

	ImGui::EndMainMenuBar();

	renderUi();
}

void InteractorGUI::renderUi()
{
	// RENDER GUI

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(draw_data);
}