#include "Window.h"
#include "Engine/Engine.h"

namespace Solus
{
	void Window::Initialize()
	{
		
	}

	void Window::Run()
	{
		gEngine->GetRenderWindow()->Run();
	}

	void Window::Update()
	{
		gEngine->GetRenderWindow()->Update();
	}

	void Window::Close()
	{
		gEngine->GetRenderWindow()->Close();
	}
}

