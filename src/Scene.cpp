#pragma once
#include "Scene.h"
#include "SystemManager.h"

Scene::Scene(SystemManager* systemManager) : m_systemManager(systemManager)
{
	// Create volume and camera
	m_volume = std::make_unique<Volume>(m_systemManager);
	m_camera = std::make_unique<Camera>(m_systemManager);
}

void Scene::updateScene()
{
	// Insert logic for scene updates
	// You should change variables directly using interaction, if possible
	// You might want to animate something (for instance rotation) -- then you could do that here
}

Volume* Scene::getVolume()
{
	return m_volume.get();
}

Camera* Scene::getCamera()
{
	return m_camera.get();
}

glm::mat4 Scene::getSceneMatrix()
{
	return m_sceneMatrix;
}