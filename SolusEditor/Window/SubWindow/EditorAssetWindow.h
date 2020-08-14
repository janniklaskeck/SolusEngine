#pragma once

#include "SubWindow.h"

#include "Render/FreeFlyCamera.h"

namespace Solus
{
	class Asset;
	class AssetSource;
	class FolderAssetSource;
	class AssetFolder;
}

namespace Editor
{
	

	class EditorAssetWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void OnFocusReceived() override;
		virtual void OnFocusLost() override;
		virtual void OnMinimized() override;
		virtual void OnMaximized() override;
		// <--

	private:
		void RenderAssetSource(Solus::FolderAssetSource* source, const char* title);

		void RenderAssetFolder(const Solus::AssetFolder* folder, const char* folderNameOverride = nullptr);

		void RenderFiles();
		void RenderFile(Solus::Asset* asset);

		void SetClickedFolder(Solus::AssetFolder* folder);
		void SetClickedAsset(Solus::Asset* asset);

		std::string clickedFolderPath;
		Solus::AssetSource* clickedAssetSource = nullptr;
		Solus::Asset* clickedAsset = nullptr;
	};

}