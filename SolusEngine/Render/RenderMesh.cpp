#include "RenderMesh.h"

#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "RenderDevice.h"
#include "Engine/Engine.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/ShaderAsset.h"

namespace Solus
{

	Solus::RenderMesh::RenderMesh()
	{
		const Asset& shaderAsset = gEngine->GetAssetManager()->GetAssetFromPath("Editor/Shader/DefaultShader.glsl");
		shaderAsset->Load();
		shader = shaderAsset.GetAs<ShaderAsset>()->GetRenderShader();
	}

	RenderMesh::~RenderMesh()
	{}

}