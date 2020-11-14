#include "RenderMesh.h"

#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "RenderDevice.h"
#include "Engine/Engine.h"

#include "AssetSystem/AssetManager.h"

namespace Solus
{

	Solus::RenderMesh::RenderMesh()
	{
		const Asset& shader = gEngine->GetAssetManager()->GetAssetFromPath("Editor/Shader/DefaultShader.glsl");
		gEngine->GetRenderDevice()->CreateShader(shader);
		owner = nullptr;
	}

	RenderMesh::~RenderMesh()
	{}
	
}