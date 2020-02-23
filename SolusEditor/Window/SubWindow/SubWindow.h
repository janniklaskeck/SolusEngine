#pragma once

namespace Editor
{

	class SubWindow
	{
	public:
		virtual void Initialize() = 0;
		virtual void Render() = 0;

		virtual void OnFocusReceived() = 0;
		virtual void OnFocusLost() = 0;

		virtual void OnMinimized() = 0;
		virtual void OnMaximized() = 0;

	protected:
		unsigned int windowFlags = 0;
	};
}