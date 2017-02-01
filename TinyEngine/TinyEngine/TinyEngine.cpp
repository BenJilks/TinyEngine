#include <iostream>
#include "TinyUtil.h"
#include <time.h>
using namespace std;
using namespace TinyEngine;

int main()
{
	TinyDisplay::Init();
	Vec2i monitor_size = TinyDisplay::GetMonitorSize();

	TinyDisplay* display = new TinyDisplay(monitor_size.x(), monitor_size.y(), "Tiny Demo Window", true);
	display->SetClearColour(Colour(0.2f, 0.2f, 0.2f, 1.0f));

	InitAssets();
	TinyWorld* render = new TinyWorld(display->GetWidth(), display->GetHeight());
	render->LoadLVLFile("TestLevel.lvl");
	
	render->UpdateFullTranformTree();
	
	float test = 0;
	while (!display->ShouldWindowClose())
	{
		if (display->IsKeyDown(GLFW_KEY_ESCAPE))
			display->Close();
		
		UpdateDebugCamera(display, render);
		render->RenderWorld();
		display->Update();
	}
	
	delete display;
	delete render;
}
