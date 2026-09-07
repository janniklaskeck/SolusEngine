#pragma once

struct SDL_Window;
struct SDL_Renderer;

namespace Solus
{

	class SWindow final
	{
	public:

		SWindow(const SString& WindowName, const int32 WindowWidth, const int32 WindowHeight);

		~SWindow();

		void SetFullscreen(const bool bUseFullscreen);

		bool IsFullscreen() const;

		void SetMouseCaptured(const bool bCaptureMouse);

		bool IsMouseCaptured() const;

		SDL_Window* GetSDLWindow() const;

		int32 GetWindowWidth() const;

		int32 GetWindowHeight() const;

		void ProcessEvent(const SDL_Event& Event);

	private:

		SDL_Window* SDLWindow = nullptr;

		SString WindowName;
		int32 Width = -1;
		int32 Height = -1;
	};

}