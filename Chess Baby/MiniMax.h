#include <thread>
#include <iostream>
#include "Timer.h"
#include "ChessBoard.h"
#include "AllLegalMoves.h"
#include "SavedGameState.h"
#ifndef MINIMAX_H
#define MINIMAX_H
int numLeaves = 0;
class MiniMax	{
	typedef void (MiniMax::*MoveFuncPtr)(const bool&, AllLegalMoves&);
	MoveFuncPtr moveGenerator;
	std::shared_ptr<ChessBoard> board;
	
	int maxDepth;
	std::vector<std::thread> moveThreads;
public:
	MiniMax(std::shared_ptr<ChessBoard>& board, const int& maxDepth, const bool willThread) {
		this->maxDepth = maxDepth;
		this->board = board;
		moveThreads.resize(2);
		if (willThread) 	{
			moveGenerator = &MiniMax::getAllPossibleMovesWithThreads;
		}
		else 	{
			moveGenerator = &MiniMax::getAllPossibleMoves;
		}
	}
	uint16_t miniMax(bool isWhiteMove) {
		//numLeaves = 0;
		uint16_t move = miniMaxSetUp(isWhiteMove);
		//std::cout << "MiniMax looked at: " << numLeaves << " leaves.\n";
		return move;
	}
private: /******PRIVATE******/
	uint16_t miniMaxSetUp(bool isWhiteMove) {
		int prevEval = 0;
		uint16_t bestMove = 0;
		AllLegalMoves moves;
		SGS savedGame(board);
		(this->*moveGenerator)(isWhiteMove, moves);

		for (int i = 0; i < 8; i++) {
			for (auto j = moves.allMoves->at(i)->begin(); j != moves.allMoves->at(i)->end(); j++) {
				board->makeAIMove(*j, isWhiteMove, 0);
				int eval = miniMaxAlgo(1, INT_MIN, INT_MAX, !isWhiteMove);
				savedGame.loadSavedState(board);
				if (isWhiteMove) {
					if (eval >= prevEval) {
						bestMove = *j;
						prevEval = eval;
					}
				}
				else {
					if (eval <= prevEval) {
						bestMove = *j;
						prevEval = eval;
					}
				}
			}
		}
		
		if (bestMove == 0) 	{ //if no move was selected. Pick first one
			for (int i = 0; i < 8; i++) 		{
				for (auto j = moves.allMoves->at(i)->begin(); j != moves.allMoves->at(i)->end(); j++) 		{
					return *j;
				}
			}
		}

		return bestMove;
	}
	int miniMaxAlgo(int depth, int alpha, int beta, bool isWhiteMove) {
		if (depth == maxDepth || board->getIsCheckMate() || board->getIsStaleMate()) {
			numLeaves++;
			return board->evaluateBoardValue(isWhiteMove);
		}
		AllLegalMoves moves;
		SGS savedGame(board);
		(this->*moveGenerator)(isWhiteMove, moves);
		if (isWhiteMove) 	{ //maximizer
			int maxEval = INT_MIN;
			for (int i = 0; i < 8; i++) {
				for (auto j = moves.allMoves->at(i)->begin(); j != moves.allMoves->at(i)->end(); j++) {
					board->makeAIMove(*j, isWhiteMove, depth);
					int eval = miniMaxAlgo(depth + 1, alpha, beta, false);
					savedGame.loadSavedState(board);
					maxEval = std::max(maxEval, eval);
					alpha = std::max(alpha, eval);
					if (beta <= alpha) {
						break;
					}
				}
				if (beta <= alpha) {
					break;
				}
			}
			return maxEval;
		}
		else 	{ //minimizer
			int minEval = INT_MAX;
			for (int i = 0; i < 8; i++) {
				for (auto j = moves.allMoves->at(i)->begin(); j != moves.allMoves->at(i)->end(); j++) {
					board->makeAIMove(*j, isWhiteMove, depth);
					int eval = miniMaxAlgo(depth + 1, alpha, beta, true);
					savedGame.loadSavedState(board);
					minEval = std::min(minEval, eval);
					beta = std::min(beta, eval);
					if (beta <= alpha) {
						break;
					}
				}
				if (beta <= alpha) {
					break;
				}
			}
			return minEval;
		}
	}
	void getAllPossibleMoves(const bool& isWhiteMove, AllLegalMoves& moves) {
		if (isWhiteMove) 	{
			getSecondHalfOfMovesWhite(moves);
			getFirstHalfOfMovesWhite(moves);
		}
		else {
			getFirstHalfOfMovesBlack(moves);
			getSecondHalfOfMovesBlack(moves);
		}
	}
	void getAllPossibleMovesWithThreads(const bool& isWhiteMove, AllLegalMoves& moves) {
		if (isWhiteMove) 	{
			moveThreads[0] = (std::thread(&MiniMax::getFirstHalfOfMovesWhite, this, std::ref(moves)));
			moveThreads[1] = (std::thread(&MiniMax::getSecondHalfOfMovesWhite, this, std::ref(moves)));
			for (auto& th : moveThreads) {
				th.join();
			}
		}
		else 	{
			moveThreads[0] = (std::thread(&MiniMax::getFirstHalfOfMovesBlack, this, std::ref(moves)));
			moveThreads[1] = (std::thread(&MiniMax::getSecondHalfOfMovesBlack, this, std::ref(moves)));

			for (auto& th : moveThreads) {
				th.join();
			}
		}
		//moveThreads.clear();
	}
	void getFirstHalfOfMovesWhite(AllLegalMoves& moves) {
		moves.allMoves->at(1) = board->queenMovesWhite();
		moves.allMoves->at(2) = board->rookMovesWhite();
		moves.allMoves->at(3) = board->bishopMovesWhite();
		
	}
	void getSecondHalfOfMovesWhite(AllLegalMoves& moves) {
		moves.allMoves->at(0) = board->kingMovesWhite();
		moves.allMoves->at(4) = board->knightMovesWhite();
		moves.allMoves->at(5) = board->pawnMovesWhite();
		moves.allMoves->at(6) = board->castlingMovesWhite();
		moves.allMoves->at(7) = board->enPassantMovesWhite();
	}

	void getFirstHalfOfMovesBlack(AllLegalMoves& moves) {
		moves.allMoves->at(1) = board->queenMovesBlack();
		moves.allMoves->at(2) = board->rookMovesBlack();
		moves.allMoves->at(3) = board->bishopMovesBlack();
	}
	void getSecondHalfOfMovesBlack(AllLegalMoves& moves) {
		moves.allMoves->at(0) = board->kingMovesBlack();
		moves.allMoves->at(4) = board->knightMovesBlack();
		moves.allMoves->at(5) = board->pawnMovesBlack();
		moves.allMoves->at(6) = board->castlingMovesBlack();
		moves.allMoves->at(7) = board->enPassantMovesBlack();
	}
};

#endif