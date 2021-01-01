#pragma once

#include "MeshAsset.generated.h"
#include "Engine/SolusEngine.h"
#include "AssetSystem/SAsset.h"

struct aiMesh;

namespace Solus
{

	struct MeshData
	{
		//MeshData();
		//~MeshData();

		//void GenerateBuffers();

		std::vector<Vec3f> vertices;
		std::vector<Vec2f> texCoords;
		std::vector<Vec3f> normals;
		std::vector<unsigned int> indices;

		//unsigned int vertexBuffer;
		//unsigned int indexBuffer;
		//unsigned int normalBuffer;
		//unsigned int uvBuffer;

		unsigned int MaterialIndex;
	};

	SOLUS_CLASS();
	class SOLUS_API MeshAsset : public SAsset
	{
		META(MeshAsset, SAsset)
	public:

		MeshAsset(const fs::path meshPath);
		virtual ~MeshAsset() = default;

		void Load() override;
		void Unload() override;

		bool Import() override;

		void LoadMesh(int index, aiMesh* mesh);

		static const std::pair<std::string, std::string> GetAssetFileTypeFilter();

		const std::vector<MeshData>& GetMeshes() const;
		const MeshData& GetFirstMesh() const;

	protected:

		std::vector<MeshData> meshes;
	};

}
