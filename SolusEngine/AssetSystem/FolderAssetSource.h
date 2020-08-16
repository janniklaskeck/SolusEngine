#pragma once
#include "AssetSource.h"

#include <memory>

namespace Solus
{
	class Asset;

	class SOLUS_API AssetFolder
	{
	public:
		AssetFolder(AssetSource* parentSource, std::string folderName, AssetFolder* _parent = nullptr);

		void Initialize();

		filepath GetRelativePath() const;

		filepath GetAbsolutePath() const;

		const std::string& GetFolderName() const;

		void Refresh();

		bool Rename(const std::string& newName) const;

		bool Delete() const;

		bool CreateChildFolder(const std::string& childFolderName) const;

		void GetChildFolders(std::vector<AssetFolder*>& folders) const;

		AssetSource* GetAssetSource() const;

		bool IsRootFolder() const;

	private:

		void GetChildFolders(std::vector<filepath>& childFolders);

		void GetChildFiles(std::vector<filepath>& childFiles);

	private:
		AssetFolder* parent;
		std::string folderName;
		std::vector<std::shared_ptr<AssetFolder>> childFolders;
		AssetSource* source = nullptr;
	};

	class SOLUS_API FolderAssetSource : public AssetSource
	{
	public:
		FolderAssetSource(std::string root);

		void Initialize() override;

		Asset* GetAsset(std::string& path);

		AssetFolder* GetRootFolder() const
		{
			return sourceRoot.get();
		}

		virtual void Refresh() override;

	private:
		std::unique_ptr<AssetFolder> sourceRoot;
	};

}
