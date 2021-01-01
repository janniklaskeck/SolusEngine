#pragma once
#include "RenderMesh.h"

namespace Solus
{
	class MeshAsset;

	struct OpenGLMeshData
	{
		~OpenGLMeshData();

		void GenerateBuffers(const MeshAsset& asset);

		unsigned int vertexBuffer = 0;
		unsigned int indexBuffer = 0;
		unsigned int normalBuffer = 0;
		unsigned int texCoordBuffer = 0;

		unsigned int indicesCount = 0;
	};

	class OpenGLMesh : public RenderMesh
	{
	public:
		OpenGLMesh();
		~OpenGLMesh();
		void Render(const Entity* owner) override;

		bool Load(MeshAsset& meshAsset) override;

	private:
		class OpenGLShader* GetOpenGLShader() const;

		OpenGLMeshData meshData;

		std::vector<class RenderTexture*> textures;

		unsigned int VAO = 0;
		unsigned int mvpUniformLoc = 0;
		unsigned int viewMatrixUniformLoc = 0;
		unsigned int modelMatrixUniformLoc = 0;
		unsigned int wsLightPositionUniformLoc = 0;

		unsigned int modelViewMatrixUniformLoc = 0;
		unsigned int textureSamplerLoc = 0;
	};
}
