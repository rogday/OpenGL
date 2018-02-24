#pragma once

#include <glad/glad.h>

#include "stb_image.h"
#include <iostream>
#include <string>

typedef unsigned int uint;

class Polygon {
  private:
	// Vertex Array Object & Vertex Buffer Object & Element Buffer Object
	uint VAO, VBO, EBO; // VAO contains VBO & EBO, VBO is for actual vertices,
						// EBO is for thier indices
	uint texture;
	int n;

  public:
	Polygon(float *triangle, uint *indices, int n, std::string &&textName)
		: n(n) {
		int width, height, nrChannels;

		// stbi_set_flip_vertically_on_load(true);

		unsigned char *data =
			stbi_load(textName.c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			std::cout << "falied to load image" << std::endl;
			throw "shit";
		}

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

		// VAO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n * 8, triangle,
					 GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * (n - 2) * 3,
					 indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
							  (void *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
							  (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
							  (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
	void draw() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (n - 2) * 3, GL_UNSIGNED_INT, 0);
	}
};