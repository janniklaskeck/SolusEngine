#include "EditorInputDevice.h"

#include "Engine/Engine.h"
#include "Render/Window.h"



namespace Solus
{
	EditorInputDevice::EditorInputDevice()
	{}


	EditorInputDevice::~EditorInputDevice()
	{}

	void EditorInputDevice::Update()
	{
		Solus::InputDevice::Update();
		
	}
}
