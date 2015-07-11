#ifndef CONSOLE_H
#define CONSOLE_H
#include "Text.h"
#include "Box.h"
#include <string>
using namespace std;

class IConsoleHandler
{
public:
	virtual void InputEntered(string text) = 0;
};

class Console
{
public:
	Console(IConsoleHandler* callbackObj) : handler(callbackObj) {};
	void Init(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int x, int y);
	void Render(SDL_Renderer* screen);
	void HandleInput(SDL_KeyboardEvent key, bool isPress);
	void HandleText(SDL_TextInputEvent textEvent);
	// Can use this for debug text, adds text to the output without triggering the callback
	void WriteLine(string text);
	
	bool enabled;
private:
	string inputString;
	string outputString;
	int width;
	int height;
	Text output;
	Text input;
	Box backing;
	IConsoleHandler* handler;
};

#endif