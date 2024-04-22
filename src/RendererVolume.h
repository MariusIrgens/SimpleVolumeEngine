// Handles rendering of volume data
// Arguably the most important class
// Modify the fragment shader to define your volumetric rendering style (res/volume-fs.glsl)

#pragma once
#include <algorithm>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/base/File.h>
#include <globjects/State.h>
#include <globjects/Texture.h>
#include <globjects/TextureHandle.h>

class SystemManager;

class RendererVolume
{
public:
	RendererVolume(SystemManager* systemManager);
	void renderFrame();

	// INTERACTIVE VARIABLES (USING ImGui)
	glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	float marchDistance = 0.01f;
	float densityMultiplier = 1.0f;
	float transferFunctionMidValue; // initialized in constructor
	float transferFunctionValueRange = 1000.0f;

private:
	SystemManager* m_systemManager;

	void setupScreenGeometry();
	void setupShaders();
	void setupVolumeTexture();
	void setupGradientTexture();
	void updateUniforms();


	// SCREEN QUAD
	std::unique_ptr<globjects::Buffer> m_screenQuadVertices = std::make_unique<globjects::Buffer>();
	std::unique_ptr<globjects::Buffer> m_screenQuadIndices = std::make_unique<globjects::Buffer>();
	std::unique_ptr<globjects::VertexArray> m_screenQuad = std::make_unique<globjects::VertexArray>();
	gl::GLsizei m_screenQuadSize;

	// DIRECT VOLUME RENDERING SHADER
	std::unique_ptr<globjects::Program> m_programVolume = std::make_unique<globjects::Program>();
	std::unique_ptr<globjects::File> m_vertexShaderSource = nullptr;
	std::unique_ptr<globjects::Shader> m_vertexShader = nullptr;
	std::unique_ptr<globjects::File> m_fragmentShaderSource = nullptr;
	std::unique_ptr<globjects::Shader> m_fragmentShader = nullptr;

	// COMPUTE SHADER (TO CALCULATE GRADIENT TEXTURE)
	std::unique_ptr<globjects::Program> m_programComputeGradient = std::make_unique<globjects::Program>();
	std::unique_ptr<globjects::File> m_computeGradientShaderSource = nullptr;
	std::unique_ptr<globjects::Shader> m_computeGradientShader = nullptr;

	// 3D TEXTURES (INPUT TO DIRECT VOLUME RENDERING SHADER)
	std::shared_ptr<globjects::Texture> m_volumeTexture;
	std::shared_ptr<globjects::Texture> m_GradientTexture;

};