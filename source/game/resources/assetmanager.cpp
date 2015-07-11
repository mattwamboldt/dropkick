#include "assetmanager.h"
AssetManager::~AssetManager()
{
	clear();
}

TTF_Font* AssetManager::loadFont(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, 16);
	if (font == NULL)
	{
		printf("Failed to load font %s! Error: %s\n", TTF_GetError());
	}
	else
	{
		assetList.push_back({ font, FONT });
	}

	return font;
}

SDL_Texture* AssetManager::loadTexture(SDL_Renderer* renderer, const char* path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		}
		else
		{
			assetList.push_back({ newTexture, TEXTURE });
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void AssetManager::clear()
{
	for (unsigned int i = 0; i < assetList.size(); ++i)
	{
		Asset asset = assetList[i];
		if (asset.type == AssetType::TEXTURE)
		{
			SDL_DestroyTexture(static_cast<SDL_Texture*>(asset.data));
		}
		else if (asset.type == AssetType::FONT)
		{
			TTF_CloseFont(static_cast<TTF_Font*>(asset.data));
		}
	}

	assetList.clear();
}