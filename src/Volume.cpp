#pragma once
#include "Volume.h"
#include "SystemManager.h"

Volume::Volume(SystemManager* systemManager) : m_systemManager(systemManager)
{
	// Load the data
	loadNifti();

	// MODEL MATRIX
	// The model matrix is used to transform the volume in the shader!
	//
	// center the volume at origin - might need adjustment for some volumes
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-0.5f, -0.5f, -0.5f));

	// Scale the model using the NIFTI header scale settings, and re-center on origin
	glm::vec3 scale = glm::vec3(m_scaleVector.x, m_scaleVector.y, m_scaleVector.z);
	m_modelMatrix = glm::scale(m_modelMatrix, scale);
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3((1.0f - m_scaleVector.x), (1.0f - m_scaleVector.y), (1.0f - m_scaleVector.z)));

	getValuesFromData();
}

void Volume::updateVolume()
{
	// Insert logic for volume updates
	// You should change variables directly using interaction, if possible
}

void Volume::loadNifti()
{
	globjects::debug() << "Please wait, loading NIFTI data...";

	// REMOVE EXISTING DATA FROM MEMORY
	nifti_image_free(m_nim);

	// READ FILE
	m_nim = nifti_image_read(niftyFileName.c_str(), 1);
	if (!m_nim) {
		globjects::debug() << "failed to read NIfTI image from '%s'\n", niftyFileName;
	}

	// READ HEADER
	m_header = nifti_read_header(niftyFileName.c_str(), NULL, 0);
	if (showHeaderOnLoad) disp_nifti_1_header(NULL, m_header); //print header

	// STORE HEADER DIMENSIONS
	m_dimensions = { (unsigned short)m_header->dim[1], (unsigned short)m_header->dim[2], (unsigned short)m_header->dim[3] };

	// Store dimensions from NIFTI header into a vector (used to scale the model matrix)
	float maxDim = std::max(m_dimensions[0], std::max(m_dimensions[1], m_dimensions[2]));
	m_scaleVector = glm::vec3((float)m_dimensions[0] / maxDim, (float)m_dimensions[1] / maxDim, (float)m_dimensions[2] / maxDim);
}

std::vector<unsigned short> Volume::getDimensions()
{
	return m_dimensions;
}

unsigned short* Volume::getVolumeData() {

	return (unsigned short*)m_nim->data;
}

glm::mat4 Volume::getModelMatrix()
{
	return m_modelMatrix;
}

void Volume::getValuesFromData()
{
	float dataSize = m_dimensions[0] * m_dimensions[1] * m_dimensions[2];
	// Find biggest value and calculate average value
	// Might be available in nifti header...
	for (int i = 0; i < dataSize; i++)
	{
		float currentValue = (float)getVolumeData()[i];
		if (currentValue > biggestDataValue)
		{
			biggestDataValue = currentValue;
		}
		averageDataValue += currentValue;
	}
	averageDataValue = averageDataValue / dataSize;

	// globjects::debug() << "Mean data value: " << averageDataValue;
	globjects::debug() << "Biggest data value: " << biggestDataValue;
}

float Volume::getAverageDataValue()
{
	return averageDataValue;
}

float Volume::getBiggestDataValue()
{
	return biggestDataValue;
}