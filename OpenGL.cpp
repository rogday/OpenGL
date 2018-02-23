#include <glad/glad.h>

#include "Shader.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <iostream>

typedef unsigned int uint;

void resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
};

void processEvents(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	/*	TODO:
		+ ShaderManager(which can add shaders in his program and then exec it)
		- Polygon(which can be created from array of indices and drawn)
		? Concave Polygon(trianglulation is codetime-consuming, fuck it)
	*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

	if (!window) {
		std::cout << "failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetWindowSizeCallback(window, resize);

	float triangle[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // top left
		0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f  // top right
	};

	uint indeces[] = {0, 1, 2, 1, 2, 3};

	// Vertex Array Object & Vertex Buffer Object & Element Buffer Object
	uint VAO, VBO, EBO; // VAO contains VBO & EBO, VBO is for actual vertices,
						// EBO is for thier indices

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces,
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	Shader shdr("vertexShader.vert", "fragmentShader.frag");

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processEvents(window);

		glClearColor(0.2f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shdr.apply();
		shdr.setFloat("time", glfwGetTime());
		shdr.setFloat("offset", sin(glfwGetTime()));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}