#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp myApp;
	myApp.Initialize(hInstance, "VGP336 - Game!", 1024, 768);

	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();
	return 0;
}