#include "SavedGameState.h"
#include "ChessBoard.h"
#include "Timer.h"
#include <tuple>
#include <thread>

#ifndef MINIMAX_H
#define MINIMAX_H

class MiniMax	{
	std::shared_ptr<ChessBoard> board;
	
	int maxDepth;
	std::vector<std::thread> moveThreads;
public:
	MiniMax(std::shared_ptr<ChessBoard>& board, const int& maxDepth) {
		this->maxDepth = maxDepth;
		this->board = board;
	}

	std::tuple<int, uint16_t> miniMaxAlgo(int currDepth, bool isWhiteMove) {
		int bestScore;
		uint16_t bestMove;
		SavedGameState svdState;
		if (board->getIsCheckMate() || currDepth == maxDepth || board->getIsStaleMate()) 	{
			bestScore = board->evaluateBoardValue();
			bestMove = NULL;
			return std::make_tuple(bestMove, bestScore);
		}
		
		if (isWhiteMove == true)
			bestScore = INT_MAX;
		else
			bestScore = INT_MIN;
		int w = 0;
		getAllPossibleMoves(isWhiteMove, svdState);
		for (int i = 0; i < 8 ; i++, w++) 		{

			for (auto j = svdState.allMoves->at(i)->begin(); j != svdState.allMoves->at(i)->end(); j++) 		{
				board->makeAIMove(*j, isWhiteMove, currDepth);
				std::tuple<int, uint16_t> move_loss = miniMaxAlgo(currDepth + 1, !isWhiteMove);
				board->undoAIMove(*j, isWhiteMove, currDepth);
				
				if (std::get<0>(move_loss) == 0) 	{ //no move found
					bestMove = *j;
				}

				if (isWhiteMove) 	{
					if (std::get<1>(move_loss) < bestScore) 	{
						bestScore = std::get<1>(move_loss);
						bestMove = std::get<0>(move_loss);
					}
				}
				else 	{
					if (std::get<1>(move_loss) > bestScore) {
						bestScore = std::get<1>(move_loss);
						bestMove = std::get<0>(move_loss);
					}
				}
			}
		}

		return std::make_tuple(bestScore, bestMove);
	}
private:
	void getAllPossibleMoves(const bool & isWhiteMove, SavedGameState& svdState) {
		if (isWhiteMove) 	{
			moveThreads.push_back(std::thread(&MiniMax::getFirstHalfOfMovesWhite, this, std::ref(svdState)));
			moveThreads.push_back(std::thread(&MiniMax::getSecondHalfOfMovesWhite, this, std::ref(svdState)));

			for (auto &th : moveThreads) {
				th.join();
			}
			moveThreads.clear();
		}
		else {
			moveThreads.push_back(std::thread(&MiniMax::getFirstHalfOfMovesBlack, this, std::ref(svdState)));
			moveThreads.push_back(std::thread(&MiniMax::getSecondHalfOfMovesBlack, this, std::ref(svdState)));

			for (auto& th : moveThreads) {
				th.join();
			}
			moveThreads.clear();
		}
	}
	void getFirstHalfOfMovesWhite(SavedGameState& svdState) {
		svdState.allMoves->at(1) = board->queenMovesWhite();
		svdState.allMoves->at(2) = board->rookMovesWhite();
		svdState.allMoves->at(3) = board->bishopMovesWhite();
		
		svdState.allMoves->at(0) = board->kingMovesWhite();
		svdState.allMoves->at(4) = board->knightMovesWhite();
		svdState.allMoves->at(5) = board->pawnMovesWhite();
		svdState.allMoves->at(6) = board->castlingMovesWhite();
		svdState.allMoves->at(7) = board->enPassantMovesWhite();
	}
	void getSecondHalfOfMovesWhite(SavedGameState& svdState) {
	}

	void getFirstHalfOfMovesBlack(SavedGameState& svdState) {
		svdState.allMoves->at(1) = board->queenMovesBlack();
		svdState.allMoves->at(2) = board->rookMovesBlack();
		svdState.allMoves->at(3) = board->bishopMovesBlack();
		
		svdState.allMoves->at(0) = board->kingMovesBlack();
		svdState.allMoves->at(4) = board->knightMovesBlack();
		svdState.allMoves->at(5) = board->pawnMovesBlack();
		svdState.allMoves->at(6) = board->castlingMovesBlack();
		svdState.allMoves->at(7) = board->enPassantMovesBlack();
	}
	void getSecondHalfOfMovesBlack(SavedGameState& svdState) {
	}
};

/*
//Adapated from "Artificial Intelligence for Games: Second Edition" by Ian Millington & John Funge (2009) p.676
//ISBN 978-0-12-374731-0

//The State class holds monolithic information about the state of the game at a given point in time
class State {
	function getMoves();			//Return all of the possible moves that can be made on this State
	function clone();				//Return an exact copy of this State
	function makeMove( move );		//Executes the given move on this State (in most games this also passes the turn to the next player)
	function evaluate( player );	//Returns the loss for 'player'
	function getCurrentPlayer();	//Returns the player whose turn it is to make a move
	function isTerminal();			//Returns whether or not this is a leaf node (no children)
	function getWinner();			//Returns whether or not this State has a winner
}

//The MiniMax algorithm requires 4 arguments:
//	state:			The state of the game to be evaluated
//	player:			The player whose turn it is for the given state
//	maxDepth:		The maximum depth of the search tree we are allowed to traverse (optional)
//	currentDepth:	The current depth of the search tree (optional)
//
//The function returns two values (combined as a Tuple)
//	bestScore:		The score of the best move
//	bestMove:		The optimal move for 'player' in the given state
function MiniMax( state , player , maxDepth , currentDepth ){
	var bestScore;
	var bestMove;

	//Recursive base-case prevents the function from running forever
	if( state.isTerminal() || currentDepth == maxDepth ){
		//Only terminal states can be evaluted
		bestScore = state.evaluate( player );
		//If the state is terminal or we cannot explore any deeper...
		//...Then there is no best 'next' move since we cannot make another move
		bestMove = null;

		//Return the score and the move
		return ( bestScore , bestMove );
	}

	//We want to minimize our own loss, so find the child state with the lowest loss
	if( state.getCurrentPlayer() == player )
		bestScore = infinity;
	//All opponents want to maximize our loss by minimizing their own (zero-sum game)
	else
		bestScore = -infinity;

	//Find all the possible moves this state can make
	foreach( move in state.getMoves() ){
		//Clone the current state
		var newState = state.clone();
		//Make the move on the clone
		newState.makeMove( move );

		//Recursive call will traverse the tree depth-first
		//The currentLoss value tells us the loss value of making currentMove
		//Remember to pass along the currentDepth + 1 otherwise we could exceed maxDepth
		( var currentLoss , var currentMove ) = MiniMax( newState , player , maxDepth , currentDepth + 1 );

		//If it is our turn, minimize our loss
		if( state.getCurrentPlayer() == player ){
			//If the given move is an improvement, it is now our preferred move
			if( currentLoss < bestScore ){
				bestScore = currentLoss;
				bestMove = currentMove;
			}
		}
*/
/*
		//If it is not our turn, we assume that our opponents will try to maximize our loss
		else{
			//If the given move is an improvement (remember- our loss is their gain), it is now our preferred move
			if( currentLoss > bestScore ){
				bestScore = currentLoss;
				bestMove = currentMove;
			}
		}
	}

	//Return the score and the move
	//These values should have been filled inside of the above for loop
	return ( bestScore , bestMove );
}
*/
#endif