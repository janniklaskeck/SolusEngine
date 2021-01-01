#pragma once
#include "RenderMesh.h"

namespace Solus
{
	class MeshAsset;

	struct OpenGLMeshData
	{
		~OpenGLMeshData();

		void GenerateBuffers(const MeshAsset& asset);

		uint32_t vertexBuffer = 0;
		uint32_t indexBuffer = 0;
		uint32_t normalBuffer = 0;
		uint32_t texCoordBuffer = 0;

		uint32_t indicesCount = 0;
		uint32_t textureCount = 0;
	};

	class OpenGLMesh : public RenderMesh
	{
	public:
		OpenGLMesh();
		~OpenGLMesh() override;
		void Render(const Entity* owner) override;

		bool Load(MeshAsset& meshAsset) override;

		void SetTexture(uint8_t index, TextureAsset& textureAsset) override;

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
