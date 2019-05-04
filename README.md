# DROPKICK

Dropkick is a tetris clone written in C++ using SDL. I went through a tetris phase after watching the [AGDQ exhibition of Tetris The Grand Master](https://www.youtube.com/watch?v=ViwDUiCzPVU)

The code architecture was based on a book by Mike McShaffry called Game Coding Complete, which in retrospect was way too much architecture for this small project. Live and learn right.

Orginal NES rules are used for the most part though it is meant to handle other variants. There are some nicities due to this, like seven bag randomization, an adjustable length (in code) piece queue, and a hold area. Theres also the start of implementing delays, such as the lock delay. Wall kicks aren't implemented yet though I investigated it a bit. Lots of good info on the tetris wiki.

## Modes
- Endless - Standard Single player, uses NES 30 Level progression that just keeps going at instant drop after 30
- Versus - Two players competing for high score. No garbage or powerups
- Co-op - Two people on one tetris board, try to survive with a friend. Pretty challenging and a bit buggy but surprisingly fun

## Controls

The game supports keyboard controls and xbox 360 pc controllers or whatever else sdl supports. xbox was all I had to test with.

| Action | Keyboard | Gamepad
| ------ | -------- | -----
| Move Piece Left/Right | Left/Right arrows | Left/Right D-Pad |
| Fall Faster | Down arrow | Down D-Pad |
| Hard Drop | Up Arrow | Up D-Pad |
| Rotate Clockwise | S or G | A or Y buttons |
| Rotate Counter-Clockwise | D or F | B or X buttons |
| Hold Piece | A or G | Left or Right Shoulder |
| Reset Game | Escape | Back Button/Select |
| Main Menu | Enter | Start |