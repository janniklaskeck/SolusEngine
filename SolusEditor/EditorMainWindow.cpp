#include "EditorMainWindow.h"
#include "EditorInputDevice.h"

#include "Input/InputDevice.h"

#include "Engine/Engine.h"

#include "Render/FreeFlyCamera.h"

#include "Object/World.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/FolderAssetSource.h"
#include "AssetSystem/Asset.h"

using namespace Solus;

namespace Editor
{

	void EditorMainWindow::Initialize()
	{
		gEngine->RegisterInputDevice(new EditorInputDevice);
	}

	void EditorMainWindow::Update()
	{
		if (!firstRun)
		{
			FreeFlyCamera* camera = gEngine->GetWorld()->SpawnEntity<FreeFlyCamera>(Vec3f(0, 0, -5), Vec3f(0, 0, 0));
			auto* entity = gEngine->GetWorld()->SpawnEntity<Entity>(Vec3f(0, 0, 0), Vec3f(0, 0, 0));
			entity->TEMP();
			firstRun = true;
		}
	}
}
