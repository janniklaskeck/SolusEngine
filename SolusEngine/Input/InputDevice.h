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

		void GetMousePosDelta(double& xDelta, double& yDelta);
		void GetMouseScrollDelta(double& xDelta, double& yDelta);

	private:
		std::unordered_map<int, int> keyState;
		double mouseXPos;
		double mouseYPos;

		double previousMouseXPos;
		double previousMouseYPos;

		std::unordered_map<int, int> mouseButtonState;
		double scrollXOffset;
		double scrollYOffset;
	};
}

