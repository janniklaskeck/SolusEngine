#include "stdafx.h"

#include "Engine.h"
#include "EditorMainWindow.h"
#include "EditorInputDevice.h"
#include "FileUtils.h"

int main(int argc, char* argv[])
{

	Solus::Engine::Initialize(new Editor::EditorMainWindow(new Editor::EditorInputDevice));
	Solus::Engine::Instance()->GetMainWindow()->Run();

	return 0;
}

