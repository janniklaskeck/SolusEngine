#include "MeshAsset.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Solus
{

	MeshAsset::MeshAsset(const fs::path meshPath)
		: SAsset(meshPath, SAssetType::MESH)
	{}

	void MeshAsset::Load()
	{
		Assimp::Importer importer;
		const char* path = "";
		const aiScene* scene = importer.ReadFile(metaData->GetSourceFilePath().string(), aiProcess_FixInfacingNormals
												 | aiProcess_JoinIdenticalVertices
												 | aiProcess_GenSmoothNormals
												 | aiProcess_FindInvalidData
												 | aiProcess_GenUVCoords
												 | aiProcess_Triangulate);
		if (!scene)
		{
			gEngine->Log(LogLevel::LogError, "Could not load OBJ model file: %s", importer.GetErrorString());
			return;
		}

		LoadMesh(scene->mMeshes[0]);
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			meshData.textureCount += material->GetTextureCount(aiTextureType_DIFFUSE);
			/*bool foundMaterial = false;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string fullPath = ".";
					fullPath += path.C_Str();
					Asset texture = gEngine->GetAssetManager()->GetAssetFromPath(fullPath);
					textures[i] = gEngine->GetRenderDevice()->CreateTexture(texture);
					bool foundMaterial = textures[i] != nullptr;
				}
			}
			if (!foundMaterial)
			{
				textures.push_back(gEngine->GetRenderDevice()->GetDefaultTexture());
			}*/
		}
	}

	void MeshAsset::LoadMesh(aiMesh* mesh)
	{
		meshData.materialIndex = mesh->mMaterialIndex;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D& vertex = mesh->mVertices[i];
			aiVector3D& normal = mesh->mNormals[i];
			aiVector3D texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D{ 0.0f, 0.0f, 0.0f };

			meshData.vertices.emplace_back(vertex.x, vertex.y, vertex.z);
			meshData.normals.emplace_back(normal.x, normal.y, normal.z);
			meshData.texCoords.emplace_back(texCoord.x, 1.0f - texCoord.y);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			for (unsigned int f = 0; f < face.mNumIndices; f++)
			{
				meshData.indices.push_back(face.mIndices[f]);
			}
		}
	}

	void MeshAsset::Unload()
	{
		meshData.Clear();
	}

	bool MeshAsset::Import()
	{
		return true;
	}

	const std::pair<std::string, std::string> MeshAsset::GetAssetFileTypeFilter()
	{
		return { "Mesh file (.obj)", "*.obj" };
	}

	const MeshData& MeshAsset::GetMesh() const
	{
		return meshData;
	}

}
