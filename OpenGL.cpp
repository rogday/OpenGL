#include "Polygon.h"
#include "Shader.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>
#include <iostream>

int main() {
	Window window(800, 600, "OpenGL");

	float triangle[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
		-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top left
		0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f  // top right
	};

	uint indices[] = {0, 1, 2, 1, 2, 3};

	Polygon poly(triangle, indices, 4, "container.jpg");

	Shader shdr("vertexShader.vert", "fragmentShader.frag");
	/* call apply() method before setting uniforms! */

	// shdr.setInt("texture1", 0);
	// shdr.setInt("texture2", 1);

	while (window.isOpen()) {
		window.processEvents();

		window.clear(0.2, 0.3, 0.7);

		auto Mouse = window.getMouse();
		auto Size = window.getSize();

		glm::quat Quaternion =
			glm::angleAxis(10.0f, glm::vec3((Mouse.x / Size.x) / sqrt(2),
											(Mouse.y / Size.y) / sqrt(2), 0.0));
		glm::mat4 R = glm::toMat4(Quaternion);

		shdr.apply();

		uint Rloc = glGetUniformLocation(shdr.getID(), "R");
		glUniformMatrix4fv(Rloc, 1, GL_FALSE, glm::value_ptr(R));

		poly.draw();

		window.display();
	}
}