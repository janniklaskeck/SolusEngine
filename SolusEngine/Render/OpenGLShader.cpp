#include "OpenGLShader.h"

#include "Engine/Engine.h"
#include "Utility/FileUtils.h"
#include "AssetSystem/AssetManager.h"
#include "AssetSystem/ShaderAsset.h"

#include <iostream>
#include <GL/gl3w.h>


namespace Solus
{

	OpenGLShader::OpenGLShader()
	{}


	OpenGLShader::~OpenGLShader()
	{}

	bool OpenGLShader::Load(const Asset& shaderAsset)
	{
		const char* vertexSource;
		const char* fragmentSource;// = fragmentShaderFile->GetCharContent();

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
			return false;
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
			return false;
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
			return false;
		}
		glValidateProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			GLchar ErrorLog[1024];
			glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
			return false;
		}
		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		return false;
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(shaderProgram);
	}

	unsigned int OpenGLShader::GetUniformLocation(const char* name)
	{
		return glGetUniformLocation(shaderProgram, name);
	}

	unsigned int OpenGLShader::GetShaderProgram() const
	{
		return shaderProgram;
	}
}
