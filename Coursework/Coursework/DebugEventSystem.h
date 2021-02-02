#pragma once
#include <string>
#include <time.h>
#include <queue>

class DebugEventSystem
{
public:
	DebugEventSystem();
	~DebugEventSystem();

	struct DebugEvent
	{
		const char* type;
		const char* eventMessage;
		const char* sourceFile;
		const char* eventTime;
	};

	void addEvent(const char* a_type, const char* a_message, const char* file);
	DebugEvent getNextEvent();

protected:
	std::queue<DebugEvent> events;
	DebugEvent defaultEvent;
};

