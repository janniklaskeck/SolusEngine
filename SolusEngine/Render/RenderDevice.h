#pragma once

#include "Engine/SolusEngine.h"

#include "Render/RenderSurface.h"
#include "Render/RenderTexture.h"
#include "Render/RenderMesh.h"
#include "Render/RenderShader.h"

#include <cstdint>
#include <vector>

namespace Solus
{
	class ShaderAsset;
	class MeshAsset;
	class TextureAsset;
	class PrimitiveDrawer;

	enum class TextureType : uint8_t
	{
		TEX_None = 0,
		TEX_DDS
	};

	class SOLUS_API RenderDevice : public SubSystem
	{
	public:

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Destroy() = 0;


		virtual void PreRenderScene() = 0;
		virtual void PostRenderScene() = 0;

		virtual RenderShader* CreateShader(ShaderAsset& shaderAsset) = 0;
		virtual RenderMesh* CreateMesh(MeshAsset& meshFileName) = 0;
		
		virtual RenderTexture* CreateTexture(TextureAsset& sourceFile, bool doLoading = true, TextureType = TextureType::TEX_DDS) = 0;
		virtual bool DestroyTexture(RenderTexture* texture) = 0;

		virtual PrimitiveDrawer* GetPrimitiveDrawer() = 0;

		virtual RenderTexture* GetDefaultTexture() = 0;
		virtual void SetRenderSurface(RenderSurface* surface = nullptr) = 0;
		virtual RenderSurface* GetCurrentRenderSurface() const = 0;

		void SetShouldRenderScene(bool shouldRender);
		bool ShouldRenderScene() const;

	protected:
		bool shouldRenderScene = true;
	};
}
