#pragma once

#include "SolusEngine.h"
#include "RenderShader.h"
#include "Entity.h"

#include <vector>

namespace Solus
{

	class SOLUS_API RenderMesh
	{
	public:
		RenderMesh();
		virtual ~RenderMesh();

		virtual void Render();

	protected:
		RenderShader* shader;

		std::vector<Vec3f> vertices;
		std::vector<unsigned int> indices;
	public:
		Entity* owner;
	};

}
