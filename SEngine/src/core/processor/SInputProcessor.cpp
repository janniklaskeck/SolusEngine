
#include "core/processor/SInputProcessor.hpp"

namespace Solus
{
	void SInputProcessor::Tick(float DeltaTime)
	{
		MouseWheelDelta = MouseWheelDeltaCache;
		MouseWheelDeltaCache = 0;
	}

	void SInputProcessor::QueueEvent(const SDL_Event& Event)
	{
		if (Event.type != SDL_EVENT_KEY_DOWN &&
			Event.type != SDL_EVENT_KEY_UP &&
			Event.type != SDL_EVENT_MOUSE_BUTTON_DOWN &&
			Event.type != SDL_EVENT_MOUSE_BUTTON_UP &&
			Event.type != SDL_EVENT_MOUSE_WHEEL)
		{
			return;
		}

		if (Event.type == SDL_EVENT_KEY_DOWN)
		{
			const SDL_Keycode KeyCode = Event.key.key;
			KeyState[KeyCode] = 1;
		}

		if (Event.type == SDL_EVENT_KEY_UP)
		{
			const SDL_Keycode KeyCode = Event.key.key;
			KeyState[KeyCode] = 0;
		}

		if (Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			MouseButtonState |= SDL_BUTTON_MASK(Event.button.button);
		}

		if (Event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			MouseButtonState &= ~SDL_BUTTON_MASK(Event.button.button);
		}

		if (Event.type == SDL_EVENT_MOUSE_WHEEL)
		{
			const bool bIsFlipped = Event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED;

			const int32 VerticalDelta = Event.wheel.integer_y;

			MouseWheelDeltaCache += bIsFlipped ? -VerticalDelta : VerticalDelta;
		}
	}

	bool SInputProcessor::IsKeyDown(const SDL_Keycode KeyCode) const
	{
		if (!KeyState.contains(KeyCode))
		{
			return false;
		}

		return KeyState.at(KeyCode);
	}

	bool SInputProcessor::IsKeyUp(const SDL_Keycode KeyCode) const
	{
		if (!KeyState.contains(KeyCode))
		{
			return false;
		}

		return !KeyState.at(KeyCode);
	}

	bool SInputProcessor::IsMouseButtonDown(const SMouseButtonCode ButtonCode) const
	{
		return (MouseButtonState & SDL_BUTTON_MASK((uint8)ButtonCode)) == 1;
		
	}

	bool SInputProcessor::IsMouseButtonUp(const SMouseButtonCode ButtonCode) const
	{
		return (MouseButtonState & SDL_BUTTON_MASK((uint8)ButtonCode)) == 0;
	}

	Vec2 SInputProcessor::GetMouseMoveDelta() const
	{
		Vec2 Pos{};
		SDL_GetRelativeMouseState(&Pos.x, &Pos.y);
		return Pos;
	}

	Vec2 SInputProcessor::GetMousePos() const
	{
		Vec2 Pos{};
		SDL_GetMouseState(&Pos.x, &Pos.y);
		return Pos;
	}

	int32 SInputProcessor::GetMouseWheelDelta() const
	{
		return MouseWheelDelta;
	}

}
