#include "stdafx.h"

#include "Engine.h"
#include "EditorMainWindow.h"
#include "FileUtils.h"

using namespace Solus;

int main(int argc, char* argv[])
{

	gEngine->Initialize();
	gEngine->InitWindow(new Editor::EditorMainWindow);
	gEngine->GetWindow()->Run();


	return 0;
}

