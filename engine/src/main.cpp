#include <application.h>

// Check for leak     _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#include <crtdbg.h>

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 720;

int main()
{
 	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		Application app = Application(SCR_WIDTH, SCR_HEIGHT);

		if (app.initialize() == 0)
			app.renderLoop();

	}

	_CrtDumpMemoryLeaks();

	return 0;
}