#include "AssetSource.h"
#include "TextAsset.h"
#include "MeshAsset.h"
#include "TextureAsset.h"

#include "Helper.h"

#include <filesystem>

namespace Solus
{

	AssetSource::AssetSource(std::string root)
	{
		this->root = std::filesystem::absolute(root);
		// Text files
		extensionTypeMap[".txt"] = AssetType::AT_TEXT;
		extensionTypeMap[".glsl"] = AssetType::AT_TEXT;

		// Texture files
		extensionTypeMap[".dds"] = AssetType::AT_TEXTURE;

		// Mesh files
		extensionTypeMap[".obj"] = AssetType::AT_MESH;
	}

	void AssetSource::InitializeAsset(std::filesystem::path relativePath)
	{
		Asset* asset;
		std::string extension = relativePath.extension().generic_string();
		if (extensionTypeMap.find(extension) != extensionTypeMap.end())
		{
			switch (extensionTypeMap[extension])
			{
			case AssetType::AT_TEXT:
				asset = new TextAsset();
				break;
			case AssetType::AT_TEXTURE:
				asset = new TextureAsset();
				break;
			case AssetType::AT_MESH:
				asset = new MeshAsset();
				break;
			default:
				asset = new TextAsset();
				break;
			}
		}
		else
		{
			asset = new TextAsset();
		}
		auto absolutePath = (root / relativePath);
		asset->Initialize(absolutePath);
		assets[relativePath.generic_string()] = asset;
	}

	void AssetSource::CleanPath(std::string& path)
	{
		if (path.rfind("./", 0) != std::string::npos)
		{
			path.erase(0, 2);
		}
		std::replace(path.begin(), path.end(), '\\', '/');
	}

}
