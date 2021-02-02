#include "DebugEventSystem.h"

DebugEventSystem::DebugEventSystem()
{
	defaultEvent.eventMessage = "No Event";
	defaultEvent.eventTime = "N/A \n";
	defaultEvent.sourceFile = "NULL";
	defaultEvent.type = "System";
}


DebugEventSystem::~DebugEventSystem()
{
}

void DebugEventSystem::addEvent(const char* a_type, const char* a_message, const char* a_file)
{
	DebugEvent e;
	e.type = a_type;
	e.eventMessage = a_message;
	e.sourceFile = a_file;

	//Get the time the event was registered
	time_t now = time(0);
	e.eventTime = ctime(&now);

	events.push(e);
}

DebugEventSystem::DebugEvent DebugEventSystem::getNextEvent()
{
	DebugEvent e;
	e = defaultEvent;

	if (events.size() > 0)
	{
		e = events.front();
		events.pop();
	}
	

	return e;

}