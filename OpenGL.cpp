#include "Polygon.h"
#include "Shader.h"
#include "Window.h"

#include <cmath>
#include <iostream>

int main() {
	Window window(800, 600, "OpenGL");

	float triangle[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // top left
		0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f  // top right
	};

	uint indices[] = {0, 1, 2, 1, 2, 3};

	Polygon poly(triangle, indices, 4);

	Shader shdr("vertexShader.vert", "fragmentShader.frag");

	while (window.isOpen()) {
		window.processEvents();

		window.clear(0.2, 0.3, 0.7);

		shdr.apply();
		shdr.setFloat("time", glfwGetTime());
		shdr.setFloat("offset", sin(glfwGetTime() * 4));

		poly.draw();

		window.display();
	}
}