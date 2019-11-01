#include "EditorMainWindow.h"
#include "InputDevice.h"
#include "Engine.h"
#include "FreeFlyCamera.h"
#include "World.h"

using namespace Solus;

namespace Editor
{

	EditorMainWindow::EditorMainWindow(InputDevice* inputDevice) : Window(inputDevice)
	{
		
	}

	EditorMainWindow::~EditorMainWindow()
	{}

	void EditorMainWindow::Update()
	{
		if (!firstRun)
		{
			FreeFlyCamera* camera = Engine::Instance()->GetWorld()->SpawnEntity<FreeFlyCamera>(Vec3f(0, 0, -3));
			auto* entity = Engine::Instance()->GetWorld()->SpawnEntity<Entity>();
			entity->TEMP();
			firstRun = true;
		}
	}
}
