// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "DebugEventSystem.h"
#include "Timer.h"
#include "AICore.h"
#include "ParameterSet.h"
#include <thread>
#include "DnDProfile.h"


class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();
	void aiThreadFunc(AICore* core);

private:
	DebugEventSystem* eventSystem;
	DebugEventSystem::DebugEvent lastEvent;
	AICore* aiCore;
	ParameterSet* set;

	DnDProfile* profile;



	float elapsedTime = 0.0f;
	float deltaTime = 0.0f;
	int eventDisplayTime = 1;

	//Timer timer;

};

#endif