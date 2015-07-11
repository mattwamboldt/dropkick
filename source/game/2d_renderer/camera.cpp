#include "camera.h"
#include <cstdio>

Camera::Camera(SDL_Renderer* renderer, int width, int height, double angle)
	:renderer(renderer), width(width), height(height), angle(angle)
{
	//Create uninitialized texture
	renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	if (renderTarget == NULL)
	{
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
	}
}

Camera::~Camera()
{
	if (renderTarget != NULL)
	{
		SDL_DestroyTexture(renderTarget);
	}
}

void Camera::PreRender()
{
	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);
}

void Camera::PostRender()
{
	SDL_SetRenderTarget(renderer, NULL);
	SDL_Point screenCenter = { width / 2, height / 2 };
	SDL_RenderCopyEx(renderer, renderTarget, NULL, NULL, angle, &screenCenter, SDL_RendererFlip::SDL_FLIP_NONE);
}