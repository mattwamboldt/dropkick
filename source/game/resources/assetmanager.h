// This will be a system to load and unload assets, like audio files, maps, images, fonts etc.
// This means that we have only one place to manage the memory for loaded data
#ifndef ASSET_MAN
#define ASSET_MAN
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <cstdio>
#include <vector>

enum AssetType
{
	TEXTURE,
	FONT
};

struct Asset
{
	void* data;
	AssetType type;
};

class AssetManager
{
public:
	~AssetManager();
	TTF_Font* loadFont(const char* path, int size);
	SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
	void clear();

private:
	std::vector<Asset> assetList;
};

#endif