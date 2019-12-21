#include "stdafx.h"

#include "Window/EditorMainWindow.h"

#include "Engine/Engine.h"

#include "Utility/FileUtils.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/FolderAssetSource.h"

using namespace Solus;

int main(int argc, char* argv[])
{
	InitializeEngine(new Editor::EditorMainWindow);
	gEngine->GetWindow()->Run();

	return 0;
}

