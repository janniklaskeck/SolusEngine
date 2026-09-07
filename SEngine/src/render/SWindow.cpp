
#include "render/SWindow.hpp"

#include <SDL3/SDL.h>
#include "core/processor/SRenderingProcessor.hpp"

namespace Solus
{

	SWindow::SWindow(const SString& WindowName, const int32 WindowWidth, const int32 WindowHeight)
	{
		this->WindowName = WindowName;
		this->Width = WindowWidth;
		this->Height = WindowHeight;

		uint64 WindowFlags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;

		SDLWindow = SDL_CreateWindow(WindowName.c_str(), WindowWidth, WindowHeight, WindowFlags);
		if (!SDLWindow)
		{
			SDL_Log("Couldn't create window: %s", SDL_GetError());
		}
	}

	SWindow::~SWindow()
	{
		SDL_DestroyWindow(SDLWindow);
	}

	void SWindow::SetFullscreen(const bool bUseFullscreen)
	{
		SDL_SetWindowFullscreen(SDLWindow, bUseFullscreen);
	}

	bool SWindow::IsFullscreen() const
	{
		uint32 Flags = SDL_GetWindowFlags(SDLWindow);
		return Flags & SDL_WINDOW_FULLSCREEN;
	}

	void SWindow::SetMouseCaptured(const bool bCaptureMouse)
	{
		SDL_SetWindowRelativeMouseMode(SDLWindow, bCaptureMouse);
	}

	bool SWindow::IsMouseCaptured() const
	{
		return SDL_GetWindowRelativeMouseMode(SDLWindow);
	}

	SDL_Window* SWindow::GetSDLWindow() const
	{
		return SDLWindow;
	}

	int32 SWindow::GetWindowWidth() const
	{
		return Width;
	}

	int32 SWindow::GetWindowHeight() const
	{
		return Height;
	}

	void SWindow::ProcessEvent(const SDL_Event& Event)
	{
		// Exposed catches all(?) kinds of window resize events
		if (Event.type != SDL_EVENT_WINDOW_EXPOSED)
		{
			return;
		}

		SDL_GetWindowSize(SDLWindow, &Width, &Height);
		gEngine->GetProcessor<SRenderingProcessor>().Reset();
	}

}

