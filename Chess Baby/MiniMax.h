#include "SavedGameState.h"
#include "ChessBoard.h"

#ifndef MINIMAX_H
#define MINIMAX_H

class MiniMax	{
	ChessBoard* board = nullptr;
	int maxDepth = 2;

	SavedGameState& miniMaxAlgo(int currDepth, SavedGameState& savedState, bool isWhiteMove) {
		if (currDepth == maxDepth) 	{
			return savedState;
		}
		
		int bestScore;
		uint16_t bestMove;


	}
};


#endif