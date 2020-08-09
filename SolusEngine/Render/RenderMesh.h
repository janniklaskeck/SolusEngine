#pragma once

#include "Engine/SolusEngine.h"
#include "RenderShader.h"
#include "RenderTexture.h"
#include "Object/Entity.h"

#include <vector>

namespace Solus
{
	class MeshAsset;

	class SOLUS_API RenderMesh
	{
	public:
		RenderMesh();
		virtual ~RenderMesh();

		virtual void Render(const Entity* owner) {};

		virtual bool Load(MeshAsset* meshAsset) = 0;

	protected:
		RenderShader* shader;
	public:
		Entity* owner;
	};

}
