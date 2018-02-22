#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

void resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
};

void processEvents(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {

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

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		processEvents(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}