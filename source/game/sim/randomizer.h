#ifndef RANDOMIZER_H
#define RANDOMIZER_H
#include "tetrimino.h"
#define BAG_SIZE 7

//This randomizer is using the tetris guideline 7 bag algorithm and may eventually add the TGM randomizr or more customization
//link: http://tetrisconcept.net/wiki/Random_Generator
namespace Tetris
{
	class Randomizer
	{
	public:
		Randomizer();
		PieceType next();
		void shuffle();

	private:
		PieceType bag[BAG_SIZE];
		int pieceIndex;
	};
}
#endif