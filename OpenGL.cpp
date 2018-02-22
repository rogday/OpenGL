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
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f,  -0.5f, 0.0f, // bottom right
		0.0f,  0.5f,  0.0f  // top
	};

	uint VBO;

	glGenBuffers(1, &VBO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle,
	// GL_STATIC_DRAW);

	// vertex shader
	std::string vertexShaderSourceString;
	const GLchar *vertexShaderSource =
		getShaderSource(vertexShaderSourceString, "vertexShader.vert");

	uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// fragment shader
	std::string fragmentShaderSourceString;
	const GLchar *fragmentShaderSource =
		getShaderSource(fragmentShaderSourceString, "fragmentShader.frag");

	uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// shader program
	uint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(shaderProgram);

	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
	//					  (void *)0);
	// glEnableVertexAttribArray(0);

	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		processEvents(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}