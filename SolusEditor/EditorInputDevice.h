#pragma once
#include "Input/InputDevice.h"

#include <vector>

namespace Solus
{
	class EditorInputDevice : public Solus::InputDevice
	{
	public:
		EditorInputDevice();
		virtual ~EditorInputDevice();

		virtual void Update() override;
	};
}

