#pragma once
#include "RenderMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Solus
{
	struct OpenGLMeshEntry
	{
		OpenGLMeshEntry();
		~OpenGLMeshEntry();

		void GenerateBuffers();

		std::vector<Vec3f> vertices;
		std::vector<Vec2f> texCoords;
		std::vector<Vec3f> normals;
		std::vector<unsigned int> indices;

		unsigned int vertexBuffer;
		unsigned int indexBuffer;
		unsigned int normalBuffer;
		unsigned int uvBuffer;

		unsigned int MaterialIndex;
	};

	class OpenGLMesh : public RenderMesh
	{
	public:
		OpenGLMesh();
		~OpenGLMesh();
		virtual void Render() override;

		virtual bool Load(MeshAsset* meshAsset) override;

	private:
		void LoadMesh(int index, aiMesh* mesh);
		class OpenGLShader* GetOpenGLShader() const;

		std::vector<OpenGLMeshEntry> entries;
		std::vector<class RenderTexture*> textures;

		unsigned int VAO = 0;
		unsigned int mvpUniformLoc = 0;
		unsigned int viewMatrixUniformLoc = 0;
		unsigned int modelMatrixUniformLoc = 0;
		unsigned int wsLightPositionUniformLoc = 0;

		unsigned int modelViewMatrixUniformLoc = 0;
		unsigned int textureSamplerLoc = 0;

		bool isLoaded = false;

	};
}
