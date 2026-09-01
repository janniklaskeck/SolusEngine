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

		SDL_Window* GetSDLWindow() const;

		int32 GetWindowWidth() const;

		int32 GetWindowHeight() const;

	private:

		SDL_Window* Window = nullptr;
		SDL_Renderer* Renderer = nullptr;

		SString WindowName;
		int32 Width = -1;
		int32 Height = -1;
	};

}