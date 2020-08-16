#include "stdafx.h"

#include "Window/EditorMainWindow.h"

#include "Engine/Engine.h"

#include "Utility/FileUtils.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/FolderAssetSource.h"

int main(int argc, char* argv[])
{
	std::string projectRootFile;
	if (argc > 1)
		projectRootFile = argv[1];
	InitializeEngine(new Solus::EditorMainWindow, projectRootFile);
	Solus::gEngine->GetWindow()->Run();

	return 0;
}

