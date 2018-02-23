#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Window {
  private:
	GLFWwindow *window;

  public:
	Window(int height, int width, std::string &&title) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window =
			glfwCreateWindow(height, width, title.c_str(), nullptr, nullptr);

		if (!window) {
			std::cout << "failed to create window" << std::endl;
			glfwTerminate();
			throw "sh1t";
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "failed to initialize GLAD" << std::endl;
			glfwTerminate();
			throw "sh1t";
		}

		glViewport(0, 0, height, width);
		glfwSetWindowSizeCallback(window, resize);
	}

	static void resize(GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	};

	void processEvents() {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	bool isOpen() { return !glfwWindowShouldClose(window); }

	void display() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void setMode(bool b) {
		if (b)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void clear(float a, float b, float c) {
		glClearColor(a, b, c, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	~Window() { glfwTerminate(); }
};