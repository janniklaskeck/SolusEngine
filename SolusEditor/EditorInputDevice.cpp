#include "EditorInputDevice.h"

#include "Engine.h"
#include "Window.h"

namespace Editor
{
	EditorInputDevice::EditorInputDevice()
	{}


	EditorInputDevice::~EditorInputDevice()
	{}

	void EditorInputDevice::Update()
	{
		Solus::InputDevice::Update();
		if (IsKeyDown(GLFW_KEY_ESCAPE))
		{
			Solus::gEngine->GetWindow()->Close();
		}
	}
}
