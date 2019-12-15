#include "AssetSource.h"
#include "TextAsset.h"
#include "MeshAsset.h"
#include "TextureAsset.h"

namespace Solus
{

	AssetSource::AssetSource(std::string root)
		: root(root)
	{
		// Text files
		extensionTypeMap[".txt"] = AssetType::AT_TEXT;
		extensionTypeMap[".glsl"] = AssetType::AT_TEXT;

		// Texture files
		extensionTypeMap[".dds"] = AssetType::AT_TEXTURE;

		// Mesh files
		extensionTypeMap[".obj"] = AssetType::AT_MESH;
	}

	void AssetSource::InitializeAsset(std::string& path, std::string extension)
	{
		CleanPath(path);
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		Asset* asset;
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
		assets[path] = asset;
		assets[path]->Initialize(path.c_str());
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
