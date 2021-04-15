#include "SavedGameState.h"
#include "ChessBoard.h"

#ifndef MINIMAX_H
#define MINIMAX_H

class MiniMax	{
	std::shared_ptr<ChessBoard> board;
	//ChessBoard* board = nullptr;
	int maxDepth;

	MiniMax(std::shared_ptr<ChessBoard>& board, const int& maxDepth) {
		this->maxDepth = maxDepth;
		this->board = board;
	}

	SavedGameState& miniMaxAlgo(int currDepth, SavedGameState& savedState, bool isWhiteMove) {
		if (currDepth == maxDepth) 	{
			return savedState;
		}
		
		int bestScore;
		uint16_t bestMove;


	}
};


#endif