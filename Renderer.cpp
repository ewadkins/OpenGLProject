/*
 * Renderer.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: ericwadkins
 */

#include "Renderer.h"
#include "OpenGLApplication.h"

Renderer::Renderer(OpenGLApplication* application) :
		_projectionMatrix(GLMatrix<float>::identity(4)) {
	_application = application;
	_shaderProgram1 = nullptr;
	vao = 0;
	vbo = 0;
	_currentProgram = nullptr;
}

// Sets up the shaders for use in rendering
void Renderer::setupShaders() {
	_shaderProgram1 = new ShaderProgram(_application, "Shader 1",
			"shaders/vertex.glsl", "shaders/fragment.glsl");
}

// Sets the shader program to be used in rendering
void Renderer::useProgram(ShaderProgram* program) {
	glUseProgram(program->getProgramId());
	_currentProgram = program;
	_application->_logger->log("Using shader program: ").log(program->getName()).endLine();
	updateUniforms();
}

// Sets the projection matrix to be used in rendering
void Renderer::setProjectionMatrix(GLMatrix<float> projectionMatrix) {
	_projectionMatrix = projectionMatrix;
	_currentProgram->setUniformMatrix4x4f("projectionMatrix",
			_projectionMatrix.getValuesArray());
}

// Updates certain uniforms upon the use of a different shader program
void Renderer::updateUniforms() {
	_currentProgram->setUniformMatrix4x4f("projectionMatrix",
			_projectionMatrix.getValuesArray());
}

// Initialization method ran on startup
void Renderer::initialize() {
	// Create shader programs
	setupShaders();

	// Attempt to assign a shader program
	if (_shaderProgram1->getProgramId() != 0)
		useProgram(_shaderProgram1);
	else
		throw "No shader assigned";

	initTriangle();
}

// Helper method that fills a vector with the relevant vertex attributes
std::vector<GLfloat> pushData(std::vector<GLfloat> data, int vertexCount,
		GLfloat vertexData[], GLfloat colorData[]) {
	for (int i = 0; i < vertexCount; i++) {
		data.push_back(vertexData[3 * i]);
		data.push_back(vertexData[3 * i + 1]);
		data.push_back(vertexData[3 * i + 2]);
		data.push_back(colorData[3 * i]);
		data.push_back(colorData[3 * i + 1]);
		data.push_back(colorData[3 * i + 2]);
	}
	return data;
}

// Test: Initializes data that will draw a triangle
void Renderer::initTriangle() {

	std::vector<GLfloat> data;

	GLfloat vertexData[] =
			{ 0.0, 0.8, -0.5, -0.8, -0.8, -0.5, 0.8, -0.8, -0.5 };
	GLfloat colorData[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, };

	// Number of vertices
	int vertexCount = sizeof(vertexData) / sizeof(vertexData[0]);

	// Add vertex data and color data to the data vector
	data = pushData(data, vertexCount, vertexData, colorData);

	// Make and bind the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Make and bind the VBO
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0],
	GL_STATIC_DRAW);

	// Add position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	// Add color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

	// Enable the attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

// Test: Renders the triangle
void Renderer::renderTriangle() {

	// Clear everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the VAO (the triangle)
	glBindVertexArray(vao);

	// Draw the VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VAO
	glBindVertexArray(0);

}

// Displays the rendered scene
void Renderer::display() {
	// Swaps the display buffers (displays what was drawn)
	glfwSwapBuffers(_application->_window);
	glfwPollEvents();
	//glfwWaitEvents();
}

