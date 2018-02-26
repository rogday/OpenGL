#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>

class Shader {
  private:
	uint shaderProgram;

  private:
	const GLchar *getShaderSource(std::string &ShaderSourceString,
								  std::string &&filename) {
		std::ifstream ShaderFile(filename);
		ShaderSourceString.assign((std::istreambuf_iterator<char>(ShaderFile)),
								  (std::istreambuf_iterator<char>()));

		return (const GLchar *)ShaderSourceString.c_str();
	}

  public:
	Shader(std::string &&vName, std::string &&fName) {
		char infoLog[512];
		int success;

		// vertex shader
		std::string vertexShaderSourceString;
		const GLchar *vertexShaderSource =
			getShaderSource(vertexShaderSourceString, vName.c_str());

		uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cout << "vertexShader compile error.\n"
					  << infoLog << std::endl;
			throw "sh1t";
		}

		// fragment shader
		std::string fragmentShaderSourceString;
		const GLchar *fragmentShaderSource =
			getShaderSource(fragmentShaderSourceString, fName.c_str());

		uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cout << "fragmentShader compile error.\n"
					  << infoLog << std::endl;
			throw "sh1t";
		}
		// shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cout << "Link error.\n" << infoLog << std::endl;
			throw "sh1t";
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	void apply() { glUseProgram(shaderProgram); }
	void setFloat(std::string &&name, float a) {
		int variable = glGetUniformLocation(shaderProgram, name.c_str());
		glUniform1f(variable, a);
	}
	void setInt(std::string &&name, int a) {
		int variable = glGetUniformLocation(shaderProgram, name.c_str());
		glUniform1i(variable, a);
	}

	uint getID() { return shaderProgram; }
};