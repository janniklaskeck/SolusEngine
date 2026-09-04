#pragma once

#include "SProcessor.hpp"

namespace Solus
{
	enum class SMouseButtonCode : uint8
	{
		None = 0,
		LeftButton = SDL_BUTTON_LEFT,
		MiddleButton = SDL_BUTTON_MIDDLE,
		RightButton = SDL_BUTTON_RIGHT,
		ExtraButton1 = SDL_BUTTON_X1,
		ExtraButton2 = SDL_BUTTON_X2
	};

	class SInputProcessor : public SProcessor
	{
	public:

		void Tick(float DeltaTime) override;

		void QueueEvent(const SDL_Event& Event);

	public:

		bool IsKeyDown(const SDL_Keycode KeyCode) const;

		bool IsKeyUp(const SDL_Keycode KeyCode) const;

		bool IsMouseButtonDown(const SMouseButtonCode ButtonCode) const;

		bool IsMouseButtonUp(const SMouseButtonCode ButtonCode) const;

		Vec2 GetMouseMoveDelta() const;

		Vec2 GetMousePos() const;

		int32 GetMouseWheelDelta() const;

	private:
		std::unordered_map<SDL_Keycode, uint8> KeyState;
		uint8 MouseButtonState = 0;
		int32 MouseWheelDelta = 0;
		int32 MouseWheelDeltaCache = 0;
	};
}