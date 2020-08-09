#pragma once

#include "MeshAsset.generated.h"
#include "Asset.h"
#include "Render/RenderMesh.h"

namespace Solus
{
	SOLUS_CLASS();
	class MeshAsset : public Asset
	{
		META(MeshAsset, Asset)

	public:
		virtual void Load() override;
		virtual void Unload() override;

		RenderMesh* GetRenderMesh() const;

	private:
		std::unique_ptr<RenderMesh> renderMesh;
	};

}
