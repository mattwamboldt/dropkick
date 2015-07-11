#include "inputmanager.h"

void InputManager::Init()
{
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		controllers[i] = NULL;
		playerDeviceMap[i].assigned = false;
		playerDeviceMap[i].controllerIndex = -1;
		playerDeviceMap[i].joystickId = -1;
	}

	int controllerIndex = 0;
	for (int i = 0; i < SDL_NumJoysticks() && controllerIndex < MAX_PLAYERS; ++i) {
		if (SDL_IsGameController(i)) {
			SDL_GameController *pad = SDL_GameControllerOpen(i);
			if (pad)
			{
				controllers[controllerIndex] = pad;
				playerDeviceMap[controllerIndex].assigned = true;
				playerDeviceMap[controllerIndex].controllerIndex = i;

				SDL_Joystick *joy = SDL_GameControllerGetJoystick(pad);
				playerDeviceMap[controllerIndex].joystickId = SDL_JoystickInstanceID(joy);

				++controllerIndex;
			}
		}
	}

	keyBinding[SDLK_LEFT] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	keyBinding[SDLK_RIGHT] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	keyBinding[SDLK_UP] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	keyBinding[SDLK_DOWN] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	keyBinding[SDLK_s] = SDL_CONTROLLER_BUTTON_A;
	keyBinding[SDLK_d] = SDL_CONTROLLER_BUTTON_B;
	keyBinding[SDLK_f] = SDL_CONTROLLER_BUTTON_X;
	keyBinding[SDLK_g] = SDL_CONTROLLER_BUTTON_Y;
	keyBinding[SDLK_ESCAPE] = SDL_CONTROLLER_BUTTON_BACK;
	keyBinding[SDLK_RETURN] = SDL_CONTROLLER_BUTTON_START;
	keyBinding[SDLK_a] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	keyBinding[SDLK_h] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
}


int InputManager::findPlayer(int joystickId)
{
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (playerDeviceMap[i].joystickId == joystickId)
		{
			return i;
		}
	}

	return -1;
}

bool InputManager::HandleEvent(TetrisGame* game, const SDL_Event& e)
{
	int player = 0;
	bool handled = true;
	switch (e.type)
	{
	case SDL_KEYUP:
		if (keyBinding.count(e.key.keysym.sym))
		{
			game->HandleRelease(keyBinding[e.key.keysym.sym], player);
		}

		break;

	case SDL_KEYDOWN:
		if (keyBinding.count(e.key.keysym.sym))
		{
			game->HandlePress(keyBinding[e.key.keysym.sym], player);
		}

		break;

	case SDL_CONTROLLERBUTTONDOWN:
		player = findPlayer(e.cbutton.which);
		if (player >= 0)
		{
			game->HandlePress((SDL_GameControllerButton)e.cbutton.button, player);
		}
		break;

	case SDL_CONTROLLERBUTTONUP:
		player = findPlayer(e.cbutton.which);
		if (player >= 0)
		{
			game->HandleRelease((SDL_GameControllerButton)e.cbutton.button, player);
		}
		break;

	default:
		handled = false;
	}

	return handled;
}