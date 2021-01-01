#pragma once
#include "RenderDevice.h"
#include "RenderTexture.h"

#include <iostream>
#include <memory>

namespace Solus
{
#define CHECK_OPENGL_ERROR() { \
	unsigned int err; \
	while ((err = glGetError()) != GL_NO_ERROR) \
	{\
	std::cout << "OPENGL ERROR: " << err << std::endl; \
            assert(0); \
	} \
}

	class OpenGLSurface;
	class OpenGLShader;
	class OpenGLPrimitiveDrawer;

	class OpenGLDevice : public RenderDevice
	{
	public:
		void Initialize() override;
		void Update() override;
		void Destroy() override;

		void PreRenderScene() override;
		void PostRenderScene() override;

		RenderShader* CreateShader(ShaderAsset& shaderAsset) override;
		RenderMesh* CreateMesh(MeshAsset& meshFileName) override;

		RenderTexture* CreateTexture(TextureAsset& sourceFile, bool doLoading = true, TextureType type = TextureType::TEX_DDS) override;
		bool DestroyTexture(RenderTexture* texture) override;

		PrimitiveDrawer* GetPrimitiveDrawer() override;

		RenderTexture* GetDefaultTexture() override;
		void SetRenderSurface(RenderSurface* surface = nullptr) override;
		RenderSurface* GetCurrentRenderSurface() const override;

	private:
		OpenGLPrimitiveDrawer* primitiveDrawer;

		OpenGLSurface* currentSurface;
		OpenGLSurface* defaultSurface;

		OpenGLShader* screenFramebufferShader;
		unsigned int screenTextureId;
		unsigned int screenQuadVertexArray;
		unsigned int screenQuadVertexBuffer;

		OpenGLShader* defaultScreenShader;

		Asset defaultTextureAsset;
	};

}
