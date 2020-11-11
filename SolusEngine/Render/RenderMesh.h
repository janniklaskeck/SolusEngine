#pragma once

#include "Engine/SolusEngine.h"
#include "RenderShader.h"
#include "RenderTexture.h"
#include "Object/Entity.h"
#include "AssetSystem/Asset.h"

#include <vector>

namespace Solus
{
	class SOLUS_API RenderMesh
	{
	public:
		RenderMesh();
		virtual ~RenderMesh();

		virtual void Render(const Entity* owner) {};

		virtual bool Load(Asset meshAsset) = 0;

	protected:
		RenderShader* shader;
	public:
		Entity* owner;
	};

}
