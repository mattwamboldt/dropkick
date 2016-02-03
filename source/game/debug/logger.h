#ifndef DEBUG_LOGGING_H
#define DEBUG_LOGGING_H

#include <SDL/SDL.h>

namespace Debug
{
	//Writes out the given message to the log file
	void log(int category, const char* fmt, ...);
	void log(const char* fmt, ...);

	//Writes out the given message to the debug console
	void console(int category, const char* fmt, ...);
	void console(const char* fmt, ...);
}

#endif