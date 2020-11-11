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
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

		virtual void PreRenderScene() override;
		virtual void PostRenderScene() override;

		virtual RenderShader* CreateShader(const Asset& shaderAsset) override;
		virtual RenderMesh* CreateMesh(const Asset& meshFileName) override;

		virtual RenderTexture* CreateTexture(const Asset& sourceFile, bool doLoading = true, TextureType type = TextureType::TEX_DDS) override;
		virtual bool DestroyTexture(RenderTexture* texture) override;

		virtual PrimitiveDrawer* GetPrimitiveDrawer() override;

		virtual RenderTexture* GetDefaultTexture();
		virtual void SetRenderSurface(RenderSurface* surface = nullptr) override;
		virtual RenderSurface* GetCurrentRenderSurface() const override;

	private:
		OpenGLPrimitiveDrawer* primitiveDrawer;

		OpenGLSurface* currentSurface;
		OpenGLSurface* defaultSurface;

		OpenGLShader* screenFramebufferShader;
		unsigned int screenTextureId;
		unsigned int screenQuadVertexArray;
		unsigned int screenQuadVertexBuffer;

		OpenGLShader* defaultScreenShader;

		std::unique_ptr<RenderTexture> defaultTexture;
	};

}
