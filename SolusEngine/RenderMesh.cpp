#include "RenderMesh.h"

#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "Engine.h"
#include "RenderDevice.h"

namespace Solus
{

	Solus::RenderMesh::RenderMesh()
	{
		TextAsset* vertexShader = (TextAsset*)gEngine->GetAssetManager()->GetAsset("editor/Shader/DefaultVertexShader.glsl");
		TextAsset* pixelShader = (TextAsset*)gEngine->GetAssetManager()->GetAsset("editor/Shader/DefaultFragmentShader.glsl");
		shader = gEngine->GetRenderDevice()->CreateShader(vertexShader, pixelShader);
		owner = nullptr;
	}

	RenderMesh::~RenderMesh()
	{}

	void Solus::RenderMesh::Render()
	{}

	



}