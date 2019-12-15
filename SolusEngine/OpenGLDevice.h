#pragma once
#include "RenderDevice.h"

#include <iostream>

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

	class OpenGLDevice : public RenderDevice
	{
	public:
		virtual void Initialize() override;

		virtual void PreRenderScene() override;
		virtual void PostRenderScene() override;

		virtual RenderShader* CreateShader(TextAsset* vertexShaderFile, TextAsset* fragmentShaderFile) override;
		virtual RenderMesh* CreateMesh(MeshAsset* meshFileName) override;

		virtual RenderTexture* CreateTexture(TextureAsset* sourceFile, bool doLoading = true, TextureType type = TextureType::TEX_DDS) override;

		virtual void SetRenderSurface(RenderSurface* surface = nullptr) override;

	private:
		OpenGLSurface* currentSurface;
		OpenGLSurface* defaultSurface;

		OpenGLShader* screenShader;
		unsigned int screenTextureId;
		unsigned int screenQuadVertexArray;
		unsigned int screenQuadVertexBuffer;
	};

}
