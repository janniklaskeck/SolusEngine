#include "OpenGLShader.h"

#include "Engine/Engine.h"
#include "Utility/FileUtils.h"
#include "AssetSystem/AssetManager.h"
#include "AssetSystem/ShaderAsset.h"

#include <iostream>
#include <GL/gl3w.h>
#include <sstream>

namespace Solus
{

	OpenGLShader::OpenGLShader()
	{}


	OpenGLShader::~OpenGLShader()
	{}

	bool OpenGLShader::Load(const Asset& shaderAsset)
	{
		ShaderAsset& asset = (ShaderAsset&)(*shaderAsset);
		asset.Load();
		const char* shaderSource = (const char*)asset.GetShaderContent();
		GLint shaderSize = asset.GetShaderSize();
		std::stringstream ss1;
		ss1 << "#version 460 core\n";
		ss1 << "#define VERTEX_SHADER\n";
		ss1 << shaderSource;
		const std::string vertexString = ss1.str();
		std::stringstream ss2;
		ss2 << "#version 460 core\n";
		ss2 << "#define FRAGMENT_SHADER\n";
		ss2 << shaderSource;
		const std::string fragmentString = ss2.str();
		const char* vertexSource = vertexString.c_str();
		const char* fragmentSource = fragmentString.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar InfoLog[1024];
			glGetShaderInfoLog(vertexShader, sizeof(InfoLog), nullptr, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
			return false;
		}
		// fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar InfoLog[1024];
			glGetShaderInfoLog(fragmentShader, sizeof(InfoLog), nullptr, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog);
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

		return true;
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
