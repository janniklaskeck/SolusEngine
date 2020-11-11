#pragma once

#include "SubWindow.h"

#include "Render/FreeFlyCamera.h"
#include "AssetSystem/Asset.h"

namespace Solus
{
	class AssetSource;
	class FolderAssetSource;
	class AssetFolder;

	class EditorAssetWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		void Initialize() override;
		void Render() override;
		void OnFocusReceived() override;
		void OnFocusLost() override;
		void OnMinimized() override;
		void OnMaximized() override;
		// <--

	private:
		void RenderAssetSource(FolderAssetSource* source, const char* title);

		//void RenderAssetFolder(const AssetFolder* folder, const char* folderNameOverride = nullptr);

		void RenderFiles();
		void RenderFile(Asset asset);

		//void SetClickedFolder(AssetFolder* folder);
		void SetClickedAsset(Asset asset);

		std::string clickedFolderPath;
		AssetSource* clickedAssetSource = nullptr;
		Asset clickedAsset;
	};

}