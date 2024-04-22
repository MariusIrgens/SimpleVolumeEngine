// Holds all scene objects
// Handles scene transform matrix
// Holds camera

#pragma once
#include "Volume.h"
#include "Camera.h"

#include <iostream>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <globjects/globjects.h>

class SystemManager;

class Scene
{
public:

	Scene(SystemManager* systemManager);
	void updateScene();
	Volume* getVolume();
	Camera* getCamera();
	glm::mat4 getSceneMatrix();

private:

	SystemManager* m_systemManager;
	std::unique_ptr<Volume> m_volume;
	std::unique_ptr<Camera> m_camera;

	// Scene Matrix
	glm::mat4 m_sceneMatrix = glm::mat4(1.0f);
	glm::mat4 m_sceneRotationMatrix = glm::mat4(1.0f);
	glm::mat4 m_sceneMatrixXr = glm::mat4(1.0f);


};

