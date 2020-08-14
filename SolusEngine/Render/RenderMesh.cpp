#include "RenderMesh.h"

#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "RenderDevice.h"
#include "Engine/Engine.h"

namespace Solus
{

	Solus::RenderMesh::RenderMesh()
	{
		TextAsset* vertexShader = (TextAsset*)gEngine->GetAssetManager()->GetAsset("Editor/Shader/DefaultVertexShader.glsl");
		TextAsset* pixelShader = (TextAsset*)gEngine->GetAssetManager()->GetAsset("Editor/Shader/DefaultFragmentShader.glsl");
		shader = gEngine->GetRenderDevice()->CreateShader(vertexShader, pixelShader);
		owner = nullptr;
	}

	RenderMesh::~RenderMesh()
	{}
	
}