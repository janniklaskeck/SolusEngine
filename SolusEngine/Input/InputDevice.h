#pragma once

#include "Engine/SolusEngine.h"
#include "InputListener.h"

#include <unordered_map>

namespace Solus
{
	class SOLUS_API InputDevice : public SubSystem
	{
	public:
		InputDevice();
		virtual ~InputDevice();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Destroy() override;

		virtual void KeyEventUpdate(int key, int scancode, int action, int mods);
		virtual void MousePosEventUpdate(double xPos, double yPos);
		virtual void MouseButtonEventUpdate(int key, int action, int mods);
		virtual void MouseScrollEventUpdate(double xOffset, double yOffset);

		bool IsKeyDown(int keyCode);
		bool IsKeyUp(int keyCode);

		bool IsButtonDown(int mouseButton);
		bool IsButtonUp(int mouseButton);

		void GetMousePosDelta(float& xDelta, float& yDelta);
		void GetMouseScrollDelta(float& xDelta, float& yDelta);

	private:
		std::unordered_map<int, int> keyState;
		float mouseXPos;
		float mouseYPos;

		float previousMouseXPos; 
		float previousMouseYPos;

		std::unordered_map<int, int> mouseButtonState;
		float scrollXOffset;
		float scrollYOffset;

		float previousScrollXOffset;
		float previousScrollYOffset;
	};
}

