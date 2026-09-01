
#include "core/processor/SInputProcessor.hpp"

namespace Solus
{
	void SInputProcessor::Tick(float DeltaTime)
	{
		MouseMoveDelta = MouseMoveDeltaCache;
		MouseMoveDeltaCache = {};
	}

	void SInputProcessor::QueueEvent(const SDL_Event& Event)
	{
		if (Event.type != SDL_EVENT_KEY_DOWN &&
			Event.type != SDL_EVENT_KEY_UP &&
			Event.type != SDL_EVENT_MOUSE_BUTTON_DOWN &&
			Event.type != SDL_EVENT_MOUSE_BUTTON_UP &&
			Event.type != SDL_EVENT_MOUSE_MOTION &&
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


		if (Event.type == SDL_EVENT_MOUSE_MOTION)
		{
			MouseMoveDeltaCache.x += std::abs(Event.motion.xrel) > 0.f ? Event.motion.xrel : 0.f;
			MouseMoveDeltaCache.y += std::abs(Event.motion.yrel) > 0.f ? Event.motion.yrel : 0.f;
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

	bool SInputProcessor::WasKeyJustPressed(const SDL_Keycode KeyCode) const
	{
		return false;
	}

	bool SInputProcessor::WasKeyJustReleased(const SDL_Keycode KeyCode) const
	{
		return false;
	}

	Vec2 SInputProcessor::GetMouseMoveDelta() const
	{
		return MouseMoveDelta;
	}

}
