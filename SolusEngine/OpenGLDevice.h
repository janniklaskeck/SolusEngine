#pragma once
#include "RenderDevice.h"

namespace Solus
{
	class OpenGLDevice : public RenderDevice
	{
		virtual RenderShader* CreateShader(const char* vertexShaderFile, const char* fragmentShaderFile);
		virtual RenderMesh* CreateMesh(const char* meshFileName);
	};

}
