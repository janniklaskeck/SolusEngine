#include "assets/SMeshAsset.hpp"

#include "assets/SAssetManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Solus
{
	void SMeshAsset::Load()
	{
		SAssetManager::Get().ReadFile(GetPath(), Bytes);

		if (IsLoaded())
		{
			return;
		}

		if (Size() == 0)
		{
			return;
		}

		Assimp::Importer Importer;
		const uint32 ImportFlags = aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType;

		const aiScene* Scene = Importer.ReadFileFromMemory(
			Data(),
			Size(),
			ImportFlags,
			"obj");
		if (!Scene)
		{
			return;
		}

		bgfx::VertexLayout VertexLayout;
		VertexLayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		aiMesh* Mesh = Scene->mMeshes[0];

		Bounds.Min = { Mesh->mAABB.mMin.x, Mesh->mAABB.mMin.y, Mesh->mAABB.mMin.z };
		Bounds.Max = { Mesh->mAABB.mMax.x, Mesh->mAABB.mMax.y, Mesh->mAABB.mMax.z };

		VertexData.reserve(Mesh->mNumVertices);

		for (int32 i = 0; i < Mesh->mNumVertices; i++)
		{
			aiVector3D& Vertex3D = Mesh->mVertices[i];
			aiVector3D& Normal = Mesh->mNormals[i];
			SVertex Vertex;
			Vertex.Position = { Vertex3D.x, Vertex3D.y, Vertex3D.z };
			Vertex.Normal = { Normal.x, Normal.y, Normal.z };
			Vertex.Color = { 255, 0, 0, 255 };

			VertexData.push_back(Vertex);
		}

		VertexBufferHandle = bgfx::createVertexBuffer(
			bgfx::makeRef(VertexData.data(), VertexData.size() * sizeof(SVertex)),
			VertexLayout
		);

		// uint16 index size
		const int32 NumIndicies = Mesh->mNumFaces * 3;
		IndexData.reserve(NumIndicies);

		for (int32 i = 0; i < Mesh->mNumFaces; i++)
		{
			aiFace& Face = Mesh->mFaces[i];
			if (Face.mNumIndices != 3)
			{
				continue;
			}

			IndexData.push_back(Face.mIndices[0]);
			IndexData.push_back(Face.mIndices[1]);
			IndexData.push_back(Face.mIndices[2]);
		}

		IndexBufferHandle = bgfx::createIndexBuffer(
			bgfx::makeRef(IndexData.data(), IndexData.size() * sizeof(uint16))
		);
	}

	void SMeshAsset::Unload()
	{
		VertexData.clear();
		IndexData.clear();

		bgfx::destroy(VertexBufferHandle);
		bgfx::destroy(IndexBufferHandle);
	}

	bool SMeshAsset::IsValid() const
	{
		return SAssetManager::Get().IsValidPath(GetPath());
	}

	bool SMeshAsset::IsLoaded() const
	{
		return bgfx::isValid(VertexBufferHandle) && bgfx::isValid(IndexBufferHandle);
	}

}
