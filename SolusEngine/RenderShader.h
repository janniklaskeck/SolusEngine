#pragma once

class RenderShader
{
public:
	RenderShader();
	virtual ~RenderShader();

	virtual bool Load(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) = 0;

	virtual unsigned int GetShaderProgram() const = 0;
};

