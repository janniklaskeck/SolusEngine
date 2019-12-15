#pragma once

#include "SolusEngine.h"
#include "RenderShader.h"
#include "RenderTexture.h"
#include "Entity.h"

#include <vector>

namespace Solus
{
	class MeshAsset;

	class SOLUS_API RenderMesh
	{
	public:
		RenderMesh();
		virtual ~RenderMesh();

		virtual void Render();

		virtual bool Load(MeshAsset* meshAsset) = 0;

	protected:
		RenderShader* shader;
	public:
		Entity* owner;
	};

}
