#pragma once
#include "Window.h"
namespace Editor
{
	class EditorMainWindow : public Solus::Window
	{
	public:

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		bool firstRun = false;
	};
}
