#pragma once

#include "SolusEngine.h"

#include <cstdint>

namespace Solus
{
	class MeshAsset;
	class TextAsset;
	class TextureAsset;
	class RenderShader;
	class RenderMesh;
	class RenderTexture;
	class RenderSurface;

	enum class TextureType : uint8_t
	{
		TEX_None = 0,
		TEX_DDS
	};

	class SOLUS_API RenderDevice
	{
	public:

		virtual void Initialize() = 0;

		virtual void PreRenderScene() = 0;
		virtual void PostRenderScene() = 0;

		virtual RenderShader* CreateShader(TextAsset* vertexShaderFile, TextAsset* fragmentShaderFile) = 0;
		virtual RenderMesh* CreateMesh(MeshAsset* meshFileName) = 0;

		virtual RenderTexture* CreateTexture(TextureAsset* sourceFile, bool doLoading = true, TextureType = TextureType::TEX_DDS) = 0;

		virtual RenderTexture* GetDefaultTexture() = 0;

		virtual void SetRenderSurface(RenderSurface* surface = nullptr) = 0;
	};
}
