#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL/SDL.h"
#include "../tetris/tetris.h"
#include <unordered_map>

//Support just two controllers for now
#define MAX_PLAYERS 4

struct PlayerDevice
{
	bool assigned = false;
	int controllerIndex = -1; // -1 is keyboard
	int joystickId = -1; // We need to save this for handling controller unplugging
};

// This class is used to manage keyboards, controllers, mappings and connects and disconnects
class InputManager
{
public:
	void Init();
	bool HandleEvent(TetrisGame* game, const SDL_Event& e);

private:
	int findPlayer(int joystickId);

	SDL_GameController* controllers[MAX_PLAYERS];
	PlayerDevice playerDeviceMap[MAX_PLAYERS];
	std::unordered_map < SDL_Keycode, SDL_GameControllerButton > keyBinding;
};

#endif