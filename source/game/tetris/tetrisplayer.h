#ifndef TETRISPLAYER_H
#define TETRISPLAYER_H

#include "../2d_renderer/Text.h"
#include "../tetris/tetrion.h"
#include "../sim/randomizer.h"
#include "../sim/tetrimino.h"
#include "../sim/settings.h"

namespace Tetris
{
	class TetrisPlayer
	{
	public:
		TetrisPlayer();

		void Init(Tetrion* playfield, int x, int y, Settings* s, SDL_Renderer* screen, TTF_Font* font, SDL_Texture** blockTextures, SDL_Texture* frameTexture, SDL_Texture* gridTexture);
		void Reset();
		void Update();
		void Render(SDL_Renderer* screen);

		void rotate(int direction);
		void hold();
		void startSoftDrop();
		void stopSoftDrop();
		void hardDrop();
		void move(int direction);

		void spawn();
		void lock();

		bool isGameOver() { return gameover; }

	private:
		void fall(int distance);

		void updateText();
		PieceType advancePreview();

		Text scoreText;
		Text lineText;
		Text gravityText;

		int score;
		int linesCleared;

		bool softDropActive;
		bool holdAvailable;

		int lockDelayFrameCount;

		//These index into our current gravity, in some games
		//the level advancement also adjusts the various delays
		//to speed the game up after 20G is reached
		int level;
		int gravity;

		int gravityCount;

		Tetrion* playfield;
		Randomizer randomizer;
		Tetromino currentPiece;

		Tetrion holdArea;
		Tetromino holdPiece;

		Tetrion preview;
		Tetrion sidePreview;
		Tetromino previewPieces[4];

		Settings* settings;

		bool gameover;
	};
}

#endif