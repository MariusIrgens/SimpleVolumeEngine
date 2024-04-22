// Holds the volume data
// Handles volume transform matrix

#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <nifti1_io.h>
#include <glm/glm.hpp>

class SystemManager;

class Volume
{
public:
	Volume(SystemManager* systemManager);
	void updateVolume();
	void loadNifti();
	std::vector<unsigned short> getDimensions();
	unsigned short* getVolumeData();
	glm::mat4 getModelMatrix();
	void getValuesFromData();
	float getAverageDataValue();
	float getBiggestDataValue();

private:
	SystemManager* m_systemManager;

	// Model matrix
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	// NIFTI
	std::string niftyFileName = "./../dat/hand.nii.gz"; // default dataset
	bool showHeaderOnLoad = false;
	nifti_image* m_nim = NULL;
	nifti_1_header* m_header;
	std::vector<unsigned short> m_dimensions = { 0, 0, 0 };
	glm::vec3 m_scaleVector = glm::vec3(1.0f);

	float biggestDataValue = 0.0f;
	float averageDataValue = 0.0f;

};