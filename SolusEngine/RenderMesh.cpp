#include "RenderMesh.h"

#include "OpenGLShader.h"
#include "Engine.h"
#include "RenderDevice.h"

namespace Solus
{

	Solus::RenderMesh::RenderMesh()
	{
		shader = Engine::Instance()->GetRenderDevice()->CreateShader("../../../SolusEngine/Resources/DefaultVertexShader.glsl", "../../../SolusEngine/Resources/DefaultFragmentShader.glsl");
	}

	RenderMesh::~RenderMesh()
	{}

	void Solus::RenderMesh::Render()
	{
		
	}

}