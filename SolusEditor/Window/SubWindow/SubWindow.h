#pragma once

namespace Solus
{

	class SubWindow
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update(float deltaTime)
		{}
		virtual void Render() = 0;

		virtual void OnFocusReceived()
		{};
		virtual void OnFocusLost()
		{};

		virtual void OnMinimized()
		{};
		virtual void OnMaximized()
		{};

	protected:
		unsigned int windowFlags = 0;
	};
}