#pragma once

#include "MeshAsset.generated.h"
#include "Engine/SolusEngine.h"
#include "AssetSystem/SAsset.h"

struct aiMesh;

namespace Solus
{

	struct MeshData
	{
		std::vector<Vec3f> vertices;
		std::vector<Vec2f> texCoords;
		std::vector<Vec3f> normals;
		std::vector<uint32_t> indices;

		uint32_t materialIndex = 0;

		uint32_t textureCount = 0;

		void Clear()
		{
			vertices.clear();
			texCoords.clear();
			normals.clear();
			indices.clear();
			materialIndex = 0;
		}
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

		void LoadMesh(aiMesh* mesh);

		static const std::pair<std::string, std::string> GetAssetFileTypeFilter();

		const MeshData& GetMesh() const;

	protected:

		MeshData meshData;
	};

}
