#pragma once

#include "SolusEngine.h"
namespace Solus
{
	struct SOLUS_API KeyEvent
	{
		int keyCode;
		int action;
		int modifier;

		KeyEvent(int _keyCode, int _action, int _modifer)
			: keyCode(_keyCode), action(_action), modifier(_modifer)
		{}
	};

	class SOLUS_API InputListener
	{
	public:
		virtual void OnKeyEvent(KeyEvent& event) = 0;
	};
}