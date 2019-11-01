#include "OpenGLShader.h"

#include "FileUtils.h"
#include <iostream>
#include <GL/gl3w.h>
namespace Solus
{

	OpenGLShader::OpenGLShader()
	{}


	OpenGLShader::~OpenGLShader()
	{}

	bool OpenGLShader::Load(const char * vertexShaderFilePath, const char * fragmentShaderFilePath)
	{
		std::string vertexShaderSource = FileUtils::ReadFile(vertexShaderFilePath);
		const GLchar* vertexSource = vertexShaderSource.c_str();

		std::string fragmentShaderSource = FileUtils::ReadFile(fragmentShaderFilePath);
		const GLchar* fragmentSource = fragmentShaderSource.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar InfoLog[1024];
			glGetShaderInfoLog(vertexShader, sizeof(InfoLog), NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
		}
		// fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar ErrorLog[1024];
			glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		}
		glValidateProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			GLchar ErrorLog[1024];
			glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
			exit(1);
		}
		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		return false;
	}

	unsigned int OpenGLShader::GetShaderProgram() const
	{
		return shaderProgram;
	}
}
