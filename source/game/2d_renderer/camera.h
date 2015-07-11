#ifndef CAMERA_H
#define CAMERA_H
#include <SDL\SDL.h>

//This will allow us to change the view independant of the world co-ordinates
class Camera
{
public:
	Camera(SDL_Renderer* renderer, int width, int height, double angle);
	~Camera();
	void PreRender(); // Sets the render target
	void PostRender(); // Renders the image
	double angle;

private:
	SDL_Texture* renderTarget; //By using a texture to render to we can rotate the camera
	SDL_Renderer* renderer;
	int width;
	int height;
};

#endif