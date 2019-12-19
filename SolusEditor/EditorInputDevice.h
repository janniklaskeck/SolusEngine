#pragma once
#include "Input/InputDevice.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace Editor
{
	class EditorInputDevice : public Solus::InputDevice
	{
	public:
		EditorInputDevice();
		virtual ~EditorInputDevice();

		virtual void Update() override;
	};
}

