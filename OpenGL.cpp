#include <glad/glad.h>

#include <GLFW/glfw3.h>
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

const GLchar *getShaderSource(std::string &ShaderSourceString,
							  std::string &&filename) {
	std::ifstream ShaderFile(filename);
	ShaderSourceString.assign((std::istreambuf_iterator<char>(ShaderFile)),
							  (std::istreambuf_iterator<char>()));

	return (const GLchar *)ShaderSourceString.c_str();
}

int main() {
	/*	TODO:
		- ShaderManager(which can add shaders in his program and then exec it)
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

	char infoLog[512];
	int success;

	// vertex shader
	std::string vertexShaderSourceString;
	const GLchar *vertexShaderSource =
		getShaderSource(vertexShaderSourceString, "vertexShader.vert");

	uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "vertexShader compile error.\n" << infoLog << std::endl;
		return -1;
	}

	// fragment shader
	std::string fragmentShaderSourceString;
	const GLchar *fragmentShaderSource =
		getShaderSource(fragmentShaderSourceString, "fragmentShader.frag");

	uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "fragmentShader compile error.\n" << infoLog << std::endl;
		return -1;
	}

	// shader program
	uint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "Link error.\n" << infoLog << std::endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float triangle[] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f,  -0.5f, 0.0f, // bottom right
		-0.5f, 0.5f,  0.0f, // top left
		0.5f,  0.5f,  0.0f  // top right
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		processEvents(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}