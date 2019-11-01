#include "OpenGLDevice.h"
#include "OpenGLShader.h"
#include "OpenGLMesh.h"

namespace Solus
{
	RenderShader* OpenGLDevice::CreateShader(const char* vertexShaderFile, const char* fragmentShaderFile)
	{
		OpenGLShader* shader = new OpenGLShader;
		shader->Load(vertexShaderFile, fragmentShaderFile);
		return shader;
	}
	RenderMesh* OpenGLDevice::CreateMesh(const char* meshFileName)
	{
		return new OpenGLMesh;
	}
}