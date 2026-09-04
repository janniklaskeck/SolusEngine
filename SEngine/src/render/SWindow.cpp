
#include "render/SWindow.hpp"

#include <SDL3/SDL.h>

namespace Solus
{

	SWindow::SWindow(const SString& WindowName, const int32 WindowWidth, const int32 WindowHeight)
	{
		this->WindowName = WindowName;
		this->Width = WindowWidth;
		this->Height = WindowHeight;

		Window = SDL_CreateWindow(WindowName.c_str(), WindowWidth, WindowHeight, SDL_WINDOW_VULKAN/*SDL_WINDOW_FULLSCREEN*/);
		if (!Window)
		{
			SDL_Log("Couldn't create window: %s", SDL_GetError());
		}

		//SDL_SetWindowRelativeMouseMode(Window, true);
	}

	SWindow::~SWindow()
	{
		SDL_DestroyWindow(Window);
	}

	void SWindow::SetFullscreen(const bool bUseFullscreen)
	{
		SDL_SetWindowFullscreen(Window, bUseFullscreen);
	}

	SDL_Window* SWindow::GetSDLWindow() const
	{
		return Window;
	}

	int32 SWindow::GetWindowWidth() const
	{
		return Width;
	}

	int32 SWindow::GetWindowHeight() const
	{
		return Height;
	}

}

