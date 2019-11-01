#pragma once

#include "SolusEngine.h"
#include "RenderShader.h"
#include "RenderMesh.h"

namespace Solus
{
	class SOLUS_API RenderDevice
	{
	public:

		virtual RenderShader* CreateShader(const char* vertexShaderFile, const char* fragmentShaderFile) = 0;
		virtual RenderMesh* CreateMesh(const char* meshFileName) = 0;
	};
}
