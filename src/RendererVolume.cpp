#pragma once
#include "RendererVolume.h"
#include "SystemManager.h"

RendererVolume::RendererVolume(SystemManager* systemManager) : m_systemManager(systemManager)
{
	// SETUP MAIN RENDER SHADER

	globjects::debug() << "\nVOLUME RENDERER DEBUG\n";

	setupScreenGeometry();
	setupShaders();
	setupVolumeTexture();
	setupGradientTexture();

	// Initialize some variables
	transferFunctionMidValue = systemManager->getScene()->getVolume()->getBiggestDataValue() / 2.0f;

}

void RendererVolume::setupScreenGeometry()
{

	// SCREEN QUAD

	float quadSize = 1.0f;

	// Screen vertices
	std::array<glm::vec2, 4> screenCoordinates = { {
		glm::vec2(-quadSize, quadSize),
		glm::vec2(-quadSize, -quadSize),
		glm::vec2(quadSize, quadSize),
		glm::vec2(quadSize, -quadSize)
	} };

	// Screen indices
	static std::array< std::array<gl::GLushort, 3>, 2> screenIndices = { {
		{0, 1, 2}, // triangle 1
		{1, 3, 2}  // triangle 2
	} };

	// Pass screen quad data to buffers
	m_screenQuadVertices->setData(screenCoordinates, gl::GL_STATIC_DRAW);
	m_screenQuadIndices->setData(screenIndices, gl::GL_STATIC_DRAW);
	m_screenQuadSize = static_cast<gl::GLsizei>(screenIndices.size() * screenIndices.front().size()); // Number of indices - passed to drawElements

	// setup and bind vertex/index buffer
	m_screenQuad->bind();
	m_screenQuad->bindElementBuffer(m_screenQuadIndices.get());
	globjects::VertexAttributeBinding* vertexBinding = m_screenQuad->binding(0);
	vertexBinding->setBuffer(m_screenQuadVertices.get(), 0, sizeof(glm::vec2));
	vertexBinding->setFormat(2, gl::GL_FLOAT);
	m_screenQuad->enable(0);
}

void RendererVolume::setupShaders()
{
	// VERTEX SHADER
	m_vertexShaderSource = globjects::Shader::sourceFromFile("./../res/volume-vs.glsl");
	m_vertexShader = globjects::Shader::create(gl::GL_VERTEX_SHADER, m_vertexShaderSource.get());

	// FRAGMENT SHADER
	m_fragmentShaderSource = globjects::Shader::sourceFromFile("./../res/volume-fs.glsl");
	m_fragmentShader = globjects::Shader::create(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());
	
	 // MAIN PROGRAM
	m_programVolume->attach(m_vertexShader.get(), m_fragmentShader.get());


}

void RendererVolume::setupVolumeTexture()
{
	// VOLUME TEXTURE

	// Get volume dimension info from header
	unsigned short uWidth, uHeight, uDepth;
	uWidth = m_systemManager->getScene()->getVolume()->getDimensions().at(0);
	uHeight = m_systemManager->getScene()->getVolume()->getDimensions().at(1);
	uDepth = m_systemManager->getScene()->getVolume()->getDimensions().at(2);

	// Create 3D Texture from data
	m_volumeTexture = globjects::Texture::create(gl::GL_TEXTURE_3D);
	m_volumeTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
	m_volumeTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
	m_volumeTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_BORDER);
	m_volumeTexture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_BORDER);
	m_volumeTexture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_BORDER);
	m_volumeTexture->image3D(0, gl::GL_R16F, glm::ivec3(uWidth, uHeight, uDepth), 0, gl::GL_RED, gl::GL_UNSIGNED_SHORT, m_systemManager->getScene()->getVolume()->getVolumeData());

	// Bind 3D texture to context
	m_programVolume->setUniform("volumeTexture", 0);
	m_volumeTexture->bindActive(0);

}

void RendererVolume::setupGradientTexture()
{
	// COMPUTE GRADIENT

	// Even thougg the gradient is not used in the default render, it is passed as a texture, and you can access it using
	// float gradientTextureValue = texture(gradientTexture, somePosition).r;

	// Get volume dimension info from header
	unsigned short uWidth, uHeight, uDepth;
	uWidth = m_systemManager->getScene()->getVolume()->getDimensions().at(0);
	uHeight = m_systemManager->getScene()->getVolume()->getDimensions().at(1);
	uDepth = m_systemManager->getScene()->getVolume()->getDimensions().at(2);

	// Program
	m_programComputeGradient = globjects::Program::create();
	m_computeGradientShaderSource = globjects::Shader::sourceFromFile("./../res/computeGradient-cs.glsl");
	m_computeGradientShader = globjects::Shader::create(gl::GL_COMPUTE_SHADER, m_computeGradientShaderSource.get());
	m_programComputeGradient->attach(m_computeGradientShader.get());
	m_programComputeGradient->use();

	// Create empty 3D Texture
	m_GradientTexture = globjects::Texture::create(gl::GL_TEXTURE_3D);
	m_GradientTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
	m_GradientTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
	m_GradientTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_BORDER);
	m_GradientTexture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_BORDER);
	m_GradientTexture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_BORDER);
	m_GradientTexture->image3D(0, gl::GL_RGBA16F, glm::ivec3(uWidth, uHeight, uDepth), 0, gl::GL_RGBA, gl::GL_UNSIGNED_SHORT, nullptr);

	// Bind 3D textures to compute shader program
	m_volumeTexture->bindImageTexture(0, 0, true, 0, gl::GL_READ_ONLY, gl::GL_R16F);
	m_GradientTexture->bindImageTexture(1, 0, true, 0, gl::GL_WRITE_ONLY, gl::GL_RGBA16F);

	// Set number of workgroups (set size in shader accordingly)
	int groupSizeX = 4, groupSizeY = 4, groupSizeZ = 4;
	int numGroupsX = ceilf(float(uWidth) / float(groupSizeX));
	int numGroupsY = ceilf(float(uHeight) / float(groupSizeY));
	int numGroupsZ = ceilf(float(uDepth) / float(groupSizeZ));

	// Compute gradients
	m_programComputeGradient->dispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
	gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
	m_programComputeGradient->release();

	// Bind 3D texture to context
	m_programVolume->setUniform("gradientTexture", 1);
	m_GradientTexture->bindActive(1);
}

void RendererVolume::updateUniforms()
{
	// UPDATE UNIFORMS

	// MATRIX
	glm::mat4 sceneMatrix = m_systemManager->getScene()->getSceneMatrix();
	glm::mat4 modelMatrix = m_systemManager->getScene()->getVolume()->getModelMatrix();
	glm::mat4 viewMatrix = m_systemManager->getScene()->getCamera()->getViewMatrix();
	glm::mat4 projectionMatrix = m_systemManager->getScene()->getCamera()->getProjectionMatrix();
	glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * sceneMatrix * modelMatrix;
	glm::mat4 inverseModelViewProjectionMatrix = inverse(modelViewProjectionMatrix);
	m_programVolume->setUniform("modelViewProjectionMatrix", modelViewProjectionMatrix);
	m_programVolume->setUniform("inverseModelViewProjectionMatrix", inverseModelViewProjectionMatrix);


	// FLOATS
	float clampedTransferFunctionValueRange = std::clamp(transferFunctionValueRange, 0.0f, m_systemManager->getScene()->getVolume()->getBiggestDataValue());
	m_programVolume->setUniform("transferFunctionValueRange", clampedTransferFunctionValueRange);
	m_programVolume->setUniform("transferFunctionMidValue", transferFunctionMidValue);
	m_programVolume->setUniform("marchDistance", marchDistance);
	m_programVolume->setUniform("densityMultiplier", densityMultiplier);
}

void RendererVolume::renderFrame()
{
	auto currentState = globjects::State::currentState();

	// Get viewport size from camera
	int viewPortSize_x = m_systemManager->getScene()->getCamera()->getViewportSize_x();
	int viewPortSize_y = m_systemManager->getScene()->getCamera()->getViewportSize_y();

	// Set OpenGL state
	gl::glDepthMask(gl::GL_TRUE);
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glDepthFunc(gl::GL_LESS);

	// Clear all buffers (color, depth and stencil)
	gl::glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	gl::glClearDepth(1.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT | gl::GL_STENCIL_BUFFER_BIT);

	// Use volume program
	m_programVolume->use();

	// Update uniforms
	updateUniforms();

	// Draw frame
	m_screenQuad->drawElements(gl::GL_TRIANGLES, m_screenQuadSize, gl::GL_UNSIGNED_SHORT, nullptr);
	m_screenQuad->unbind();

	// Release volume program
	m_programVolume->release();

	currentState->apply();
}