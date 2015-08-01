#include "tetrisplayer.h"
#include "types.h"
#include <cstdio>

namespace Tetris
{
	//We're basing the initial timings off of the Nes Tetris which did
	//not have lock delay http://tetrisconcept.net/wiki/Tetris_(NES,_Nintendo)
	//TODO: pull speed curve into separate class/files to allow multiple
	struct FallData
	{
		int level;
		int frames;
		int distance;
	};

	FallData nesSpeedCurve[] = {
		{ 0, 48, 1 },
		{ 1, 43, 1 },
		{ 2, 38, 1 },
		{ 3, 33, 1 },
		{ 4, 28, 1 },
		{ 5, 23, 1 },
		{ 6, 18, 1 },
		{ 7, 13, 1 },
		{ 8, 8, 1 },
		{ 9, 6, 1 },
		{ 10, 5, 1 },
		{ 13, 4, 1 },
		{ 16, 3, 1 },
		{ 19, 2, 1 },
		{ 29, 1, 1 }
	};

	TetrisPlayer::TetrisPlayer()
	{
		softDropActive = false;
	}

	void TetrisPlayer::Init(Tetrion* field, int x, int y, Settings* s, SDL_Renderer* screen, TTF_Font* font, SDL_Texture** blockTextures, SDL_Texture* frameTexture, SDL_Texture* gridTexture)
	{
		playfield = field;
		settings = s;
		softDropActive = false;

		int fieldWidth = MINO_SIZE * FIELD_WIDTH;

		currentPiece.setField(playfield->GetField());
		currentPiece.kickEnabled = false;

		holdArea.Init(4, 2, x + fieldWidth / 2 - 7 * MINO_SIZE, y - 3 * MINO_SIZE, blockTextures, frameTexture);
		holdPiece.setField(holdArea.GetField());

		preview.Init(4, 2, x + fieldWidth / 2 - 2 * MINO_SIZE, y - 3 * MINO_SIZE, blockTextures, frameTexture);
		sidePreview.Init(4, 12, x + fieldWidth + MINO_SIZE, y, blockTextures, frameTexture);
		previewPieces[0].setField(preview.GetField());
		previewPieces[1].setField(sidePreview.GetField());
		previewPieces[2].setField(sidePreview.GetField());
		previewPieces[3].setField(sidePreview.GetField());

		previewPieces[0].x = 0;
		previewPieces[0].y = 0;

		for (int i = 0; i < 3; ++i)
		{
			previewPieces[i + 1].x = 0;
			previewPieces[i + 1].y = i * 4;
		}

		int textY = y;
		int textX = x - 150;
		scoreText.Init(screen, "SCORE 0", font, textX, textY);
		textY += scoreText.Height() + 5;

		lineText.Init(screen, "LINES 0", font, textX, textY);
		textY += lineText.Height() + 5;

		gravityText.Init(screen, "GRAVITY 0", font, textX, textY);
	}

	void TetrisPlayer::Reset()
	{
		gravity = 0;
		level = 0;
		gravityCount = 0;
		linesCleared = 0;
		score = 0;
		holdAvailable = true;
		gameover = false;

		holdPiece.x = 0;
		holdPiece.y = 0;
		holdPiece.lift();
		holdPiece.type = NONE;
		holdPiece.locktimer = -1;
		holdPiece.orientation = SPAWN;
		holdPiece.place();

		for (int i = 0; i < 4; ++i)
		{
			previewPieces[i].lift();
			previewPieces[i].type = randomizer.next();
			previewPieces[i].locktimer = -1;
			previewPieces[i].orientation = SPAWN;
			previewPieces[i].place();
		}

		updateText();

		playfield->GetField()->Clear();
	}

	void TetrisPlayer::Update()
	{
		if (currentPiece.locktimer >= 0)
		{
			currentPiece.locktimer++;
			if (currentPiece.locktimer >= settings->lockDelayFrames)
			{
				lock();
			}
		}
		else
		{
			++gravityCount;

			int gFrames = nesSpeedCurve[gravity].frames;
			int gDistance = nesSpeedCurve[gravity].distance;

			if (softDropActive && (settings->softDropDistance / (float)settings->softDropFrames) > (gDistance / (float)gFrames))
			{
				gFrames = settings->softDropFrames;
				gDistance = settings->softDropDistance;
			}

			if (gravityCount >= gFrames)
			{
				fall(gDistance);
				gravityCount = 0;
			}
		}
	}

	PieceType TetrisPlayer::advancePreview()
	{
		PieceType nextPiece = previewPieces[0].type;

		for (int i = 0; i < 4; ++i)
		{
			previewPieces[i].lift();

			if (i == 3)
			{
				previewPieces[i].type = randomizer.next();
			}
			else
			{
				previewPieces[i].type = previewPieces[i + 1].type;
			}

			previewPieces[i].locktimer = -1;
			previewPieces[i].orientation = SPAWN;
			previewPieces[i].place();
		}

		return nextPiece;
	}

	void TetrisPlayer::Render(SDL_Renderer* screen)
	{
		if (settings->holdEnabled)
		{
			holdArea.Render(screen);
		}

		if (settings->previewEnabled)
		{
			preview.Render(screen);
			sidePreview.Render(screen);
		}

		scoreText.Render(screen);
		lineText.Render(screen);
		gravityText.Render(screen);
	}

	void TetrisPlayer::updateText()
	{
		//update the score
		char text[32];
		sprintf(text, "SCORE %d", score);
		scoreText.SetText(text);

		sprintf(text, "LINES %d", linesCleared);
		lineText.SetText(text);

		sprintf(text, "GRAVITY %d/%d", nesSpeedCurve[gravity].distance, nesSpeedCurve[gravity].frames);
		gravityText.SetText(text);
	}

	void TetrisPlayer::rotate(int direction)
	{
		if (currentPiece.rotate(direction))
		{
			currentPiece.locktimer = -1;
		}
	}

	void TetrisPlayer::hold()
	{
		if (holdAvailable)
		{
			currentPiece.lift();

			holdAvailable = false;
			PieceType temp = holdPiece.type;

			holdPiece.x = 0;
			holdPiece.y = 0;
			holdPiece.lift();
			holdPiece.type = currentPiece.type;
			holdPiece.locktimer = -1;
			holdPiece.orientation = SPAWN;
			holdPiece.place();

			currentPiece.type = temp;

			if (currentPiece.type == NONE)
			{
				currentPiece.type = advancePreview();
			}

			currentPiece.x = 3;
			currentPiece.y = 0;
			currentPiece.locktimer = -1;
			currentPiece.orientation = SPAWN;
			gameover = currentPiece.isBlockout();
			currentPiece.place();
		}
	}

	void TetrisPlayer::startSoftDrop()
	{
		softDropActive = true;
	}

	void TetrisPlayer::stopSoftDrop()
	{
		softDropActive = false;
	}

	void TetrisPlayer::hardDrop()
	{
		fall(20);
	}

	void TetrisPlayer::move(int direction)
	{
		if (currentPiece.shift(direction))
		{
			currentPiece.locktimer = -1;
		}
	}

	//TODO: separate starting position from piece configuration
	void TetrisPlayer::spawn()
	{
		currentPiece.x = 3;
		currentPiece.y = 0;
		currentPiece.type = advancePreview();
		currentPiece.locktimer = -1;
		currentPiece.orientation = SPAWN;

		gameover = currentPiece.isBlockout();
		currentPiece.place();
	}

	void TetrisPlayer::fall(int distance)
	{
		//Clear the field at the current position
		currentPiece.lift();

		//move down and test if we are off the board/intersecting existing stuff
		int newY;
		bool hitBottom = false;

		for (newY = currentPiece.y + 1; newY <= currentPiece.y + distance; ++newY)
		{
			hitBottom = !currentPiece.isValidPlacement(currentPiece.x, newY);

			if (hitBottom)
			{
				break;
			}
		}

		currentPiece.y = newY - 1;

		//Put the piece back on the board at the new/same position
		currentPiece.place();

		if (hitBottom)
		{
			currentPiece.locktimer = 0;
		}
	}

	void TetrisPlayer::lock()
	{
		currentPiece.locktimer = -1;
		int numLines = playfield->GetField()->CheckLines();

		// http://tetrisconcept.net/wiki/Scoring
		//The scoring system here is just using the number of lines cleared like nes tetris
		//It's missing points for soft drop, but that's fairly inconsequential at this stage
		//eventually expand this to handle other scoring systems based on events: splits, spin detections, that kinda stuff

		switch (numLines)
		{
		case 1:
			score += (level + 1) * 40;
			break;
		case 2:
			score += (level + 1) * 100;
			break;
		case 3:
			score += (level + 1) * 300;
			break;
		case 4:
			score += (level + 1) * 1200;
			break;
		}

		linesCleared += numLines;

		if (numLines)
		{
			int currentLevel = level;
			level = linesCleared / 10;

			//We need to look up the next gravity
			//TODO replace the hard coded size of the fall data with a data struct or some shit
			if (level > currentLevel && gravity < 14)
			{
				if (level >= nesSpeedCurve[gravity + 1].level)
				{
					++gravity;
				}
			}

			updateText();
		}

		playfield->GetField()->RemoveCleared();
		holdAvailable = true;

		spawn();
	}
}