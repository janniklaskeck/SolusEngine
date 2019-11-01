#pragma once
#include "Window.h"
namespace Editor
{
	class EditorMainWindow : public Solus::Window
	{
	public:
		EditorMainWindow(class Solus::InputDevice* inputDevice);
		~EditorMainWindow();

	protected:
		virtual void Update() override;

	private:
		bool firstRun = false;
	};
}
