#include "OpenGLDevice.h"

#include "OpenGLShader.h"
#include "OpenGLMesh.h"
#include "OpenGLTexture.h"
#include "OpenGLSurface.h"
#include "OpenGLPrimitiveDrawer.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/ShaderAsset.h"

#include "Engine/Engine.h"

#include <GL/gl3w.h>

namespace Solus
{
	void APIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (type == GL_DEBUG_TYPE_ERROR)
			gEngine->Log(LogLevel::LogError, "GL ERROR: severity = 0x%x, message = %s", severity, message);
	}

	void OpenGLDevice::Initialize()
	{
		CHECK_OPENGL_ERROR();
		glEnable(GL_DEBUG_OUTPUT);
		CHECK_OPENGL_ERROR();
		glDebugMessageCallback(GLErrorCallback, 0);
		CHECK_OPENGL_ERROR();
		glEnable(GL_CULL_FACE);
		glClearColor(0.f, 0.f, 1.f, 0.f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		CHECK_OPENGL_ERROR();
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		primitiveDrawer = new OpenGLPrimitiveDrawer;

		defaultSurface = new OpenGLSurface;
		Vec2i size = gEngine->GetWindow()->GetWindowSize();
		defaultSurface->Initialize(size.x, size.y);
		SetRenderSurface(defaultSurface);

		AssetManager* assetManager = gEngine->GetAssetManager();
		const fs::path screenShaderPath = assetManager->GetEngineAssetSource()->GetRootPath() / "Editor/Shader/ScreenShader.glsl";
		Asset screenShader = assetManager->ImportAsset<ShaderAsset>(screenShaderPath);
		assert(screenShader.IsValid());

		screenFramebufferShader = (OpenGLShader*)CreateShader(screenShader);
		screenTextureId = glGetUniformLocation(screenFramebufferShader->GetShaderProgram(), "screenTexture");
		float quadVertices[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			-1.0f,  1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
		};

		// screen quad VAO
		glGenVertexArrays(1, &screenQuadVertexArray);
		glBindVertexArray(screenQuadVertexArray);

		glGenBuffers(1, &screenQuadVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glBindVertexArray(0);
		CHECK_OPENGL_ERROR();

		const fs::path defaultScreenShaderPath = assetManager->GetEngineAssetSource()->GetRootPath() / "Editor/Shader/DefaultScreenShader.glsl";
		screenShader = assetManager->ImportAsset<ShaderAsset>(defaultScreenShaderPath);
		assert(screenShader.IsValid());

		defaultScreenShader = (OpenGLShader*)CreateShader(screenShader);
	}

	void OpenGLDevice::Update()
	{}

	void OpenGLDevice::Destroy()
	{}

	void OpenGLDevice::PreRenderScene()
	{
		currentSurface->BindBuffers();
		CHECK_OPENGL_ERROR();
	}

	void OpenGLDevice::PostRenderScene()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Vec2i size = gEngine->GetWindow()->GetWindowSize();
		glViewport(0, 0, size.x, size.y);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (ShouldRenderScene())
		{
			glBindVertexArray(screenQuadVertexArray);
			glUseProgram(screenFramebufferShader->GetShaderProgram());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, currentSurface->GetColorTexture());
			glUniform1i(screenTextureId, 0);

			glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertexBuffer);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisableVertexAttribArray(0);
		}
		CHECK_OPENGL_ERROR();
	}

	RenderShader* OpenGLDevice::CreateShader(const Asset& shaderAsset)
	{
		if (!shaderAsset.IsValid())
			return nullptr;
		OpenGLShader* shader = new OpenGLShader;
		if (!shader->Load(shaderAsset))
		{
			delete shader;
			return nullptr;
		}
		return shader;
	}

	RenderMesh* OpenGLDevice::CreateMesh(const Asset& meshFile)
	{
		auto* mesh = new OpenGLMesh;
		mesh->Load(meshFile);
		return mesh;
	}

	RenderTexture* OpenGLDevice::CreateTexture(const Asset& textureFile, bool doLoading /*= true*/, TextureType type /* = TextureType::TEX_DDS */)
	{
		RenderTexture* newTexture = new OpenGLTexture(GL_TEXTURE_2D);
		if (doLoading)
		{
			if (!newTexture->Load(textureFile))
			{
				delete newTexture;
				return nullptr;
			}
		}

		return newTexture;
	}

	bool OpenGLDevice::DestroyTexture(RenderTexture* texture)
	{
		if (texture != GetDefaultTexture())
		{
			delete texture;
			return true;
		}
		return false;
	}

	PrimitiveDrawer* OpenGLDevice::GetPrimitiveDrawer()
	{
		return primitiveDrawer;
	}

	RenderTexture* OpenGLDevice::GetDefaultTexture()
	{
		if (!defaultTexture.get())
		{
			const Asset& defaultTextureAsset = gEngine->GetAssetManager()->GetAssetFromPath("Editor/texture/defaultTexture.dds");
			defaultTexture.reset(new OpenGLTexture(GL_TEXTURE_2D));
			defaultTexture.get()->Load(defaultTextureAsset);
		}

		return defaultTexture.get();
	}

	void OpenGLDevice::SetRenderSurface(RenderSurface* surface /*= nullptr*/)
	{
		if (!surface)
			currentSurface = defaultSurface;
		currentSurface = (OpenGLSurface*)surface;
	}

	RenderSurface* OpenGLDevice::GetCurrentRenderSurface() const
	{
		return currentSurface;
	}
}