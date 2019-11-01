#pragma once

#include "SolusEngine.h"

#include "Engine.h"
#include "World.h"
#include "Window.h"

#include <string>

namespace Solus
{

	class SOLUS_API GlobalManager
	{
		GlobalManager();

	protected:
		virtual ~GlobalManager();
	public:
		static GlobalManager* Instance()
		{
			if (!instance)
				instance = new GlobalManager;
			return instance;
		}

		//////////////////////////////////////////////////////////////////////////
		// Rendering
		//////////////////////////////////////////////////////////////////////////
		

	private:
		static GlobalManager* instance;

		
	};
}
