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

	// vertex coordinates in 3D plane, color, texture coordinates
	// float triangle[] = {
	// 	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
	// 	0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
	// 	-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top left
	// 	0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f  // top right
	// };
	// uint indices[] = {0, 1, 2, 1, 2, 3};
	// Polygon poly(triangle, indices, 4, "container.jpg");

	float cube[24];

	for (int i = 0; i < 8; ++i) {
		for (int k = 0; k < 3; ++k) {
			cube[i * 3 + k] = (((i & (1 << k)) != 0) - 0.5);
			std::cout << cube[i * 3 + k] << ' ';
		}
		std::cout << std::endl;
	}

	uint indices[] = {
		0, 1, 2, 1, 2, 3, // z=0 /3
		4, 5, 6, 5, 6, 7, // z=1 /3
		0, 1, 4, 1, 4, 5, // y=0 /3
		2, 3, 6, 3, 6, 7, // y=1 /3
		0, 2, 4, 2, 4, 6, // x=0 /3
		1, 3, 5, 3, 5, 7  // x=1 /3
	};

	int width, height, nrChannels;

	// stbi_set_flip_vertically_on_load(true);

	unsigned char *data =
		stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (!data) {
		std::cout << "falied to load image" << std::endl;
		throw "shit";
	}

	uint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	uint VAO, VBO, EBO;
	// VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// fuck off

	Shader shdr("vertexShader.vert", "fragmentShader.frag");
	/* call apply() method before setting uniforms! */

	// shdr.setInt("texture1", 0);
	// shdr.setInt("texture2", 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (window.isOpen()) {
		window.processEvents();

		window.clear(0.2, 0.3, 0.7);

		// glm::quat Quaternion = glm::quat(cos((float)glfwGetTime()),
		//								 glm::normalize(glm::vec3(1, 0, 1)));
		// glm::mat4 R = glm::toMat4(Quaternion);

		auto Mouse = window.getMouse();
		auto Size = window.getSize();

		glm::mat4 R;
		R = glm::rotate(R, (float)acos(2 * Mouse.x / Size.x - 1),
						glm::normalize(glm::vec3(0, 0, 1)));

		R = glm::rotate(R, (float)acos(2 * Mouse.y / Size.y - 1),
						glm::normalize(glm::vec3(1, 1, 0)));

		shdr.apply();

		uint Rloc = glGetUniformLocation(shdr.getID(), "R");
		glUniformMatrix4fv(Rloc, 1, GL_FALSE, glm::value_ptr(R));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(uint),
					   GL_UNSIGNED_INT, 0);

		// poly.draw();

		window.display();
	}
}