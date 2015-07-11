#ifndef TEXT_H
#define TEXT_H

#include "SDL/SDL_ttf.h"

enum TextAlign
{
	LEFT = 0,
	CENTER,
	RIGHT
};

class Text
{
public:
	Text() : renderedResult(NULL), font(NULL) {}
	~Text();
	void Init(SDL_Renderer* inRenderer, char* messsage, TTF_Font* font, int x, int y, SDL_Color color = { 255, 255, 255, 255 }, TextAlign align = TextAlign::LEFT);
	void SetWrap(Uint32 width);
	void Clear();
	void SetText(const char* message);
	void SetPosition(int x, int y);
	void Render(SDL_Renderer* screen);
	int Height();
	int LineHeight();

private:
	void Realign();
	TTF_Font* font;
	SDL_Color color;
	SDL_Rect position; //This is where the text is on the screen
	SDL_Rect renderRect; //This is where the surface is drawn after alignment
	TextAlign align;
	SDL_Texture* renderedResult;
	SDL_Renderer* renderer;
	Uint32 wrapWidth;

	//Image dimensions
	int mWidth;
	int mHeight;
};

#endif