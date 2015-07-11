#include "randomizer.h"
#include <random>

namespace Tetris
{
	Randomizer::Randomizer()
	{
		shuffle();
		pieceIndex = 0;
	}

	PieceType Randomizer::next()
	{
		PieceType nextPiece = bag[pieceIndex];
		pieceIndex++;
		if (pieceIndex >= BAG_SIZE)
		{
			pieceIndex = 0;
			shuffle();
		}

		return nextPiece;
	}

	void Randomizer::shuffle()
	{
		//First we fill another bag with all possible pieces
		PieceType selectableBag[NUM_PIECES];
		for (int i = 0; i < NUM_PIECES; ++i)
		{
			selectableBag[i] = (PieceType)i;
		}

		int remainingSelections = NUM_PIECES;
		int bagIndex = 0;

		//The plan is to go through the number of pieces left and choose a random slot for each between 0 and the remaining count
		//then decrement the count. this will ensure all get filled randomly
		while (remainingSelections)
		{
			//we choose a slot which will be how many
			int selection = rand() % remainingSelections;
			bag[bagIndex] = selectableBag[selection];
			
			//TODO: use a linked list or a vector or something that sizes down more easily
			//A better algo would be a list of piece type counts where the piece type is removed from a vector when all pieces are gone

			//shift down the rest of the bag
			for (int i = selection; i < remainingSelections - 1; ++i)
			{
				selectableBag[i] = selectableBag[i + 1];
			}

			--remainingSelections;
			++bagIndex;
		}
	}
}