#include "MeshAsset.h"

#include "Engine/Engine.h"

namespace Solus
{
	SCLASS_IMPL(MeshAsset);

	void MeshAsset::Load()
	{
		Asset::Load();
		if (!renderMesh.get())
			renderMesh.reset(gEngine->GetRenderDevice()->CreateMesh(this));
	}

	void MeshAsset::Unload()
	{
		Asset::Unload();
		renderMesh.reset(nullptr);
	}

	RenderMesh* MeshAsset::GetRenderMesh() const
	{
		return renderMesh.get();
	}
}