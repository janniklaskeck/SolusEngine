#pragma once

#include "SProcessor.hpp"

namespace Solus
{
	class SInputProcessor : public SProcessor
	{
	public:

		void Tick(float DeltaTime) override;

		void QueueEvent(const SDL_Event& Event);

	public:

		bool IsKeyDown(const SDL_Keycode KeyCode) const;

		bool IsKeyUp(const SDL_Keycode KeyCode) const;

		bool WasKeyJustPressed(const SDL_Keycode KeyCode) const;

		bool WasKeyJustReleased(const SDL_Keycode KeyCode) const;

		Vec2 GetMouseMoveDelta() const;

	private:
		std::unordered_map<SDL_Keycode, uint8> KeyState;

		Vec2 MouseMoveDelta{};
		Vec2 MouseMoveDeltaCache{};
	};
}