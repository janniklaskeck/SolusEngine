#include "OpenGLDevice.h"

#include "OpenGLShader.h"
#include "OpenGLMesh.h"
#include "OpenGLTexture.h"
#include "OpenGLSurface.h"

#include "TextAsset.h"
#include "MeshAsset.h"
#include "TextureAsset.h"

#include "Engine.h"
#include "AssetManager.h"

#include <GL/gl3w.h>

namespace Solus
{
	void APIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		gEngine->Log(type == GL_DEBUG_TYPE_ERROR ? LogError : LogVerbose, "GL MSG: type = 0x%x, severity = 0x%x, message = %s", type, severity, message);
	}

	void OpenGLDevice::Initialize()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(GLErrorCallback, 0);

		glEnable(GL_CULL_FACE);
		glClearColor(0.f, 0.f, 4.f, 0.f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		defaultSurface = new OpenGLSurface;
		int width, height;
		gEngine->GetRenderWindow()->GetWindowSize(&width, &height);
		defaultSurface->Initialize(width, height);
		SetRenderSurface(defaultSurface);

		TextAsset* screenVertexShader = (TextAsset*)gEngine->GetAssetManager()->GetAsset("Shader/ScreenVertexShader.glsl");
		TextAsset* screenFragmentShader = (TextAsset*)gEngine->GetAssetManager()->GetAsset("Shader/ScreenFragmentShader.glsl");

		screenShader = (OpenGLShader*)CreateShader(screenVertexShader, screenFragmentShader);
		screenTextureId = glGetUniformLocation(screenShader->GetShaderProgram(), "screenTexture");
		float quadVertices[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
		};

		// screen quad VAO
		glGenVertexArrays(1, &screenQuadVertexArray);
		glGenBuffers(1, &screenQuadVertexBuffer);
		glBindVertexArray(screenQuadVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindVertexArray(0);
		CHECK_OPENGL_ERROR();
	}

	void OpenGLDevice::PreRenderScene()
	{
		currentSurface->BindBuffers();
		CHECK_OPENGL_ERROR();
	}

	void OpenGLDevice::PostRenderScene()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1280, 720);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.f, 1.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(screenShader->GetShaderProgram());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, currentSurface->GetColorTexture());
		glUniform1i(screenTextureId, 0);

		//glBindVertexArray(screenQuadVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertexBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);

		CHECK_OPENGL_ERROR();
	}

	RenderShader* OpenGLDevice::CreateShader(TextAsset* vertexShaderFile, TextAsset* fragmentShaderFile)
	{
		OpenGLShader* shader = new OpenGLShader;
		shader->Load(vertexShaderFile, fragmentShaderFile);
		return shader;
	}

	RenderMesh* OpenGLDevice::CreateMesh(MeshAsset* meshFile)
	{
		auto* mesh = new OpenGLMesh;
		mesh->Load(meshFile);
		return mesh;
	}

	RenderTexture* OpenGLDevice::CreateTexture(TextureAsset* textureFile, bool doLoading /*= true*/, TextureType type /* = TextureType::TEX_DDS */)
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

	RenderTexture* OpenGLDevice::GetDefaultTexture()
	{
		if (!defaultTexture.get())
		{
			auto* defaultTextureAsset = (TextureAsset*)gEngine->GetAssetManager()->GetAsset("texture/defaultTexture.dds");
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
}