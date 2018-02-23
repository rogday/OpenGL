#pragma once

#include <glad/glad.h>

typedef unsigned int uint;

class Polygon {
  private:
	// Vertex Array Object & Vertex Buffer Object & Element Buffer Object
	uint VAO, VBO, EBO; // VAO contains VBO & EBO, VBO is for actual vertices,
						// EBO is for thier indices
	int n;

  public:
	Polygon(float *triangle, uint *indices, int n) : n(n) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n * 6, triangle,
					 GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * (n - 2) * 3,
					 indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
							  (void *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
							  (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
	void draw() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (n - 2) * 3, GL_UNSIGNED_INT, 0);
	}
};