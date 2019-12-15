#include "EditorMainWindow.h"
#include "InputDevice.h"
#include "Engine.h"
#include "FreeFlyCamera.h"
#include "World.h"
#include "AssetManager.h"
#include "FolderAssetSource.h"
#include "Asset.h"
#include "EditorInputDevice.h"

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
