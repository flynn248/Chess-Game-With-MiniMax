#include <memory>
#include "ChessBoard.h"
#include <iostream>
#ifndef SAVEDGAMESTATE_H
#define SAVEDGAMESTATE_H

class SGS 				{
private:
	//In retrospect, could have saved time by keeping the same name as in the board and just use this
	//Oops
	unsigned long long cpyBitBoard = 0ULL;
	unsigned long long cpyWhPieces = 0ULL;
	unsigned long long cpyBlPieces = 0ULL;

	unsigned long long cpyWhKing = 0ULL;
	unsigned long long cpyWhRook = 0ULL;
	unsigned long long cpyWhPawn = 0ULL;
	unsigned long long cpyWhQueen = 0ULL;
	unsigned long long cpyWhKnight = 0ULL;
	unsigned long long cpyWhBishop = 0ULL;

	unsigned long long cpyBlKing = 0ULL;
	unsigned long long cpyBlRook = 0ULL;
	unsigned long long cpyBlPawn = 0ULL;
	unsigned long long cpyBlQueen = 0ULL;
	unsigned long long cpyBlKnight = 0ULL;
	unsigned long long cpyBlBishop = 0ULL;

	unsigned long long cpyEnPassantWhite = 0ULL; //spots white can enPassant
	unsigned long long cpyEnPassantBlack = 0ULL; //spots black can enPassant

	unsigned long long cpyAttackedSquares = 0ULL;
	unsigned long long cpySquaresWhiteAttacks = 0ULL;
	unsigned long long cpySquaresBlackAttacks = 0ULL;

	unsigned long long cpyWhAttackKing = 0ULL;
	unsigned long long cpyWhAttackRook = 0ULL;
	unsigned long long cpyWhAttackPawn = 0ULL;
	unsigned long long cpyWhAttackQueen = 0ULL;
	unsigned long long cpyWhAttackKnight = 0ULL;
	unsigned long long cpyWhAttackBishop = 0ULL;

	unsigned long long cpyBlAttackKing = 0ULL;
	unsigned long long cpyBlAttackRook = 0ULL;
	unsigned long long cpyBlAttackPawn = 0ULL;
	unsigned long long cpyBlAttackQueen = 0ULL;
	unsigned long long cpyBlAttackKnight = 0ULL;
	unsigned long long cpyBlAttackBishop = 0ULL;

	unsigned long long cpyAttackSquaresKing = 0ULL;
	unsigned long long cpyAttackSquaresRook = 0ULL;
	unsigned long long cpyAttackSquaresPawn = 0ULL;
	unsigned long long cpyAttackSquaresQueen = 0ULL;
	unsigned long long cpyAttackSquaresKnight = 0ULL;
	unsigned long long cpyAttackSquaresBishop = 0ULL;
	unsigned long long cpyAttackSquaresEnPassant = 0ULL;

	unsigned long long cpyNotCapturable = 0ULL;
	unsigned long long cpyPinnedPiecesBitBoard = 0ULL;
	unsigned long long cpyCheckPathXRayThroughKing = 0ULL;
	unsigned long long cpyEnemyPiecesThatAreDefended = 0ULL;
	unsigned long long cpyLocationOfPieceAttackingKing = 0ULL;
	unsigned long long cpySquaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF;

	int cpyCastleRooks[4] = { 63, 56, 7, 0 };

	int cpyNumberOfChecks = 0;

	bool isCheckMate = false,
		 isStaleMate = false,
		 isWhiteMove = true,
		 hasKingMoved = false;

	unsigned long long squaresTheEnemyAttacks = 0ULL;

public:
	SGS(std::shared_ptr<ChessBoard>&);
	void loadSavedState(std::shared_ptr<ChessBoard>&);
};

SGS::SGS(std::shared_ptr<ChessBoard>& board) {
	cpyBitBoard = board->bitBoard;
	cpyWhPieces = board->whPieces;
	cpyBlPieces = board->blPieces;

	cpyWhKing = board->whKing;
	cpyWhRook = board->whRook;
	cpyWhPawn = board->whPawn;
	cpyWhQueen = board->whQueen;
	cpyWhKnight = board->whKnight;
	cpyWhBishop = board->whBishop;

	cpyBlKing = board->blKing;
	cpyBlRook = board->blRook;
	cpyBlPawn = board->blPawn;
	cpyBlQueen = board->blQueen;
	cpyBlKnight = board->blKnight;
	cpyBlBishop = board->blBishop;

	cpyEnPassantWhite = board->enPassantWhite;
	cpyEnPassantBlack = board->enPassantBlack;

	cpyAttackedSquares = board->attackedSquares;
	cpySquaresWhiteAttacks = board->squaresWhiteAttacks;
	cpySquaresBlackAttacks = board->squaresBlackAttacks;

	cpyWhAttackKing = board->whAttackKing;
	cpyWhAttackRook = board->whAttackRook;
	cpyWhAttackPawn = board->whAttackPawn;
	cpyWhAttackQueen = board->whAttackQueen;
	cpyWhAttackKnight = board->whAttackKnight;
	cpyWhAttackBishop = board->whAttackBishop;

	cpyBlAttackKing = board->blAttackKing;
	cpyBlAttackRook = board->blAttackRook;
	cpyBlAttackPawn = board->blAttackPawn;
	cpyBlAttackQueen = board->blAttackQueen;
	cpyBlAttackKnight = board->blAttackKnight;
	cpyBlAttackBishop = board->blAttackBishop;

	cpyAttackSquaresKing = board->attackSquaresKing;
	cpyAttackSquaresRook = board->attackSquaresRook;
	cpyAttackSquaresPawn = board->attackSquaresPawn;
	cpyAttackSquaresQueen = board->attackSquaresQueen;
	cpyAttackSquaresKnight = board->attackSquaresKnight;
	cpyAttackSquaresBishop = board->attackSquaresBishop;
	cpyAttackSquaresEnPassant = board->attackSquaresEnPassant;

	cpyNotCapturable = board->notCapturable;
	cpyPinnedPiecesBitBoard = board->pinnedPiecesBitBoard;
	cpyCheckPathXRayThroughKing = board->checkPathXRayThroughKing;
	cpyEnemyPiecesThatAreDefended = board->enemyPiecesThatAreDefended;
	cpyLocationOfPieceAttackingKing = board->locationOfPieceAttackingKing;
	cpySquaresToBlockCheckOrCapture = board->squaresToBlockCheckOrCapture;

	for (int i = 0; i < 4; i++) {
		cpyCastleRooks[i] = board->castleRooks[i];
	}

	cpyNumberOfChecks = board->numberOfChecks;

	isCheckMate = board->getIsCheckMate();
	isStaleMate = board->getIsStaleMate();
	isWhiteMove = board->getIsWhiteMove();
	if (isWhiteMove) {
		hasKingMoved = board->getWhiteKingPiece()->getHasKingMoved();
		squaresTheEnemyAttacks = board->getWhiteKingPiece()->getSquaresTheEnemyAttacks();
	}
	else {
		hasKingMoved = board->getBlackKingPiece()->getHasKingMoved();
		squaresTheEnemyAttacks = board->getBlackKingPiece()->getSquaresTheEnemyAttacks();
	}

}

void SGS::loadSavedState(std::shared_ptr<ChessBoard>& board) {
	board->bitBoard = cpyBitBoard;
	board->whPieces = cpyWhPieces;
	board->blPieces = cpyBlPieces;

	board->whKing = cpyWhKing;
	board->whRook = cpyWhRook;
	board->whPawn = cpyWhPawn;
	board->whQueen = cpyWhQueen;
	board->whKnight = cpyWhKnight;
	board->whBishop = cpyWhBishop;

	board->blKing = cpyBlKing;
	board->blRook = cpyBlRook;
	board->blPawn = cpyBlPawn;
	board->blQueen = cpyBlQueen;
	board->blKnight = cpyBlKnight;
	board->blBishop = cpyBlBishop;

	board->enPassantWhite = cpyEnPassantWhite;
	board->enPassantBlack = cpyEnPassantBlack;

	board->attackedSquares = cpyAttackedSquares;
	board->squaresWhiteAttacks = cpySquaresWhiteAttacks;
	board->squaresBlackAttacks = cpySquaresBlackAttacks;

	board->whAttackKing = cpyWhAttackKing;
	board->whAttackRook = cpyWhAttackRook;
	board->whAttackPawn = cpyWhAttackPawn;
	board->whAttackQueen = cpyWhAttackQueen;
	board->whAttackKnight = cpyWhAttackKnight;
	board->whAttackBishop = cpyWhAttackBishop;

	board->blAttackKing = cpyBlAttackKing;
	board->blAttackRook = cpyBlAttackRook;
	board->blAttackPawn = cpyBlAttackPawn;
	board->blAttackQueen = cpyBlAttackQueen;
	board->blAttackKnight = cpyBlAttackKnight;
	board->blAttackBishop = cpyBlAttackBishop;

	board->attackSquaresKing = cpyAttackSquaresKing;
	board->attackSquaresRook = cpyAttackSquaresRook;
	board->attackSquaresPawn = cpyAttackSquaresPawn;
	board->attackSquaresQueen = cpyAttackSquaresQueen;
	board->attackSquaresKnight = cpyAttackSquaresKnight;
	board->attackSquaresBishop = cpyAttackSquaresBishop;
	board->attackSquaresEnPassant = cpyAttackSquaresEnPassant;

	board->notCapturable = cpyNotCapturable;
	board->pinnedPiecesBitBoard = cpyPinnedPiecesBitBoard;
	board->checkPathXRayThroughKing = cpyCheckPathXRayThroughKing;
	board->enemyPiecesThatAreDefended = cpyEnemyPiecesThatAreDefended;
	board->locationOfPieceAttackingKing = cpyLocationOfPieceAttackingKing;
	board->squaresToBlockCheckOrCapture = cpySquaresToBlockCheckOrCapture;

	for (int i = 0; i < 4; i++) {
		board->castleRooks[i] = cpyCastleRooks[i];
	}

	board->numberOfChecks = cpyNumberOfChecks;

	board->setIsCheckMate(isCheckMate);
	board->setIsStaleMate(isStaleMate);
	board->setIsWhiteMove(isWhiteMove);

	if (isWhiteMove) {
		board->getWhiteKingPiece()->setHasKingMoved(hasKingMoved);
		board->getWhiteKingPiece()->setSquaresTheEnemyAttacks(squaresTheEnemyAttacks);
	}
	else {
		board->getBlackKingPiece()->setHasKingMoved(hasKingMoved);
		board->getBlackKingPiece()->setSquaresTheEnemyAttacks(squaresTheEnemyAttacks);
	}
}

#endif // !SAVEDGAMESTATE

