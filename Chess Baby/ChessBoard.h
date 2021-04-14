#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <SFML/Graphics.hpp>
#include <bitset>
#include <vector>
#include <time.h>
#include <memory>
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "Timer.h"
#include "BoardInfo.h"
#include "Stack.h"

class ChessBoard : public BoardInfo{
protected:
	static const int BSIZE = 8;
	float tileWidth;
	bool isWhiteMove; //true for white move, false for black
	bool isCheckMate = false,
		isStaleMate = false;
	int timesWhiteKingMoved = 0, //act like a semaphore for the black king movement. Once king is committed to moving, it cannot castle again
		timesWhCastledLeft = 0,
		timesWhCastledRight = 0,
		timesBlackKingMoved = 0,
		timesBlCastledRight = 0,
		timesBlCastledLeft = 0;

	sf::RectangleShape board[BSIZE][BSIZE];
	sf::RectangleShape tileSquare;
	sf::Color whtTileColor,
		blkTileColor;

	King* whKingPiece = nullptr;
	Pawn* whPawnPiece = nullptr;
	Rook* whRookPiece = nullptr;
	Queen* whQueenPiece = nullptr;
	Knight* whKnightPiece = nullptr;
	Bishop* whBishopPiece = nullptr;

	King* blKingPiece = nullptr;
	Pawn* blPawnPiece = nullptr;
	Rook* blRookPiece = nullptr;
	Queen* blQueenPiece = nullptr;
	Knight* blKnightPiece = nullptr;
	Bishop* blBishopPiece = nullptr;

	std::unique_ptr<Stack> capturedPiecesAI;

public:											  
												  
	ChessBoard(){
		isWhiteMove = true;
		tileWidth = 0;
	}								  
												  
	ChessBoard(const float &tileWidth, sf::Color & whiteCol, sf::Color & blkCol, const float &frstSquareCentRef, const int& stackSize) {
		this->tileWidth = tileWidth;
		whtTileColor = whiteCol;
		blkTileColor = blkCol;
		isWhiteMove = true;
		capturedPiecesAI = std::make_unique<Stack>(stackSize);
		initializePiecePosition(frstSquareCentRef);
	}
												  
	~ChessBoard() {
		delete whRookPiece;
		delete whKnightPiece;
		delete whBishopPiece;
		delete whQueenPiece;
		delete whKingPiece;
		delete whPawnPiece;

		delete blRookPiece;
		delete blKnightPiece;
		delete blBishopPiece;
		delete blQueenPiece;
		delete blKingPiece;
		delete blPawnPiece;

		whKingPiece = nullptr;
		whPawnPiece = nullptr;
		whRookPiece = nullptr;
		whQueenPiece = nullptr;
		whKnightPiece = nullptr;
		whBishopPiece = nullptr;

		blKingPiece = nullptr;
		blPawnPiece = nullptr;
		blRookPiece = nullptr;
		blQueenPiece = nullptr;
		blKnightPiece = nullptr;
		blBishopPiece = nullptr;
	}						  
												  
	void initializePiecePosition(const float& frstSquareCentRef) {
		
		setStartPosition();

		const float pieceScale = (1 - (tileWidth / (tileWidth * 8))) * 0.40;
		float whiteYOffset = frstSquareCentRef + (tileWidth  * 7);

		const int kingVal = 10000,
			queenVal = 900,
			rookVal = 500,
			knightVal = 300,
			bishopVal = 300,
			pawnVal = 100;
		
		whRookPiece = new Rook(rookVal, pieceScale, "assets/white-rook.png", frstSquareCentRef, true, "R", whRook, 2);
		whKnightPiece = new Knight(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef, true, "N", whKnight, 2);
		whBishopPiece = new Bishop(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef, true, "B", whBishop, 2);
		whQueenPiece = new Queen(queenVal, pieceScale, "assets/white-queen.png", frstSquareCentRef, true, "Q", whQueen,	1);
		whKingPiece = new King(kingVal, pieceScale, "assets/white-king.png", frstSquareCentRef, true, "K", whKing, 1);
		whPawnPiece = new Pawn(pawnVal, pieceScale, "assets/white-pawn.png", frstSquareCentRef, true, "P", whPawn, 8);

		blRookPiece = new Rook(rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef, false, "r", blRook, 2);
		blKnightPiece = new Knight(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef, false, "n", blKnight, 2);
		blBishopPiece = new Bishop(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef, false, "b", blBishop, 2);
		blQueenPiece = new Queen(queenVal * -1, pieceScale, "assets/black-queen.png", frstSquareCentRef, false, "q", blQueen, 1);
		blKingPiece = new King(kingVal * -1, pieceScale, "assets/black-king.png", frstSquareCentRef, false, "k", blKing, 1);
		blPawnPiece = new Pawn(pawnVal * -1, pieceScale, "assets/black-pawn.png", frstSquareCentRef, false, "p", blPawn, 8);
	}

	void drawBoard(sf::RenderWindow& window) {
		tileSquare.setSize(sf::Vector2f(tileWidth, tileWidth));

		for (int i = 0; i < BSIZE; i++) {
			for (int j = 0; j < BSIZE; j++) {
				board[i][j] = tileSquare;
				tileSquare.setPosition(sf::Vector2f(tileWidth * i, tileWidth * j));
				if ((i + j) % 2) {
					tileSquare.setFillColor(blkTileColor);
				}
				else {

					tileSquare.setFillColor(whtTileColor);
				}
				window.draw(board[i][j]);
			}
		}

		drawPieces(window);
	}
	void drawPieces(sf::RenderWindow& window) {
		whKingPiece->printSprite(window);
		whPawnPiece->printSprite(window);
		whRookPiece->printSprite(window);
		whQueenPiece->printSprite(window);
		whKnightPiece->printSprite(window);
		whBishopPiece->printSprite(window);

		blKingPiece->printSprite(window);
		blPawnPiece->printSprite(window);
		blRookPiece->printSprite(window);
		blQueenPiece->printSprite(window);
		blKnightPiece->printSprite(window);
		blBishopPiece->printSprite(window);
	}
	void printBitBoard(const unsigned long long & aBitBoard) {

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				std::cout << std::bitset<1>(aBitBoard >> (i*8 + j)) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
												  
/*
	std::vector<uint16_t>* whiteMoves() { //perhaps pass in vectors when called for each piece. Treat the moves individually to avoid the copying overhead issue
		std::unique_ptr<std::vector<uint16_t>> allPossibleMoves = new std::vector<uint16_t>;
		notCapturable = ~(whPieces | blKing); //avoid illegal capture of black king
		std::vector<uint16_t> *bishopM, *queenM, *kingM, *knightM, *pawnM, *rookM, *castleM;
		attackedSquares = 0ULL;

		bishopM = bishopMoves(whBishop);
		queenM = queenMoves(whQueen);
		kingM = kingMoves(whKing);
		knightM = knightMoves(whKnight);
		pawnM = pawnMovesWhite(whPawn);
		rookM = rookMoves(whRook);

		squaresWhiteAttacks = attackedSquares;
		//allPossibleMoves->reserve(bishopM->size() + queenM->size() + kingM->size() + knightM->size() + pawnM->size() + rookM->size());
		//allPossibleMoves->insert(allPossibleMoves->end(), bishopM->begin(), bishopM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), queenM->begin(), queenM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), kingM->begin(), kingM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), knightM->begin(), knightM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), pawnM->begin(), pawnM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), rookM->begin(), rookM->end());
		delete bishopM;
		delete queenM;
		delete kingM;
		delete knightM;
		delete pawnM;
		delete rookM;
		return allPossibleMoves;
	}  
												  
	std::unique_ptr<std::vector<uint16_t>> blackMoves() {
		std::vector<uint16_t> *allPossibleMoves = new std::vector<uint16_t>;
		notCapturable = ~(blPieces | whKing); //avoid illegal capture of white king
		std::unique_ptr<std::vector<uint16_t>> bishopM, * queenM, * kingM, * knightM, * pawnM, * rookM, * castleM;
		attackedSquares = 0ULL;

		bishopM = bishopMoves(blBishop);
		queenM = queenMoves(blQueen);
		kingM = kingMoves(blKing);
		knightM = knightMoves(blKnight);
		pawnM = pawnMovesBlack(blPawn);
		rookM = rookMoves(blRook);

		squaresBlackAttacks = attackedSquares;
		//allPossibleMoves->reserve(bishopM->size() + queenM->size() + kingM->size() + knightM->size() + pawnM->size() + rookM->size());
		//allPossibleMoves->insert(allPossibleMoves->end(), bishopM->begin(), bishopM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), queenM->begin(), queenM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), kingM->begin(), kingM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), knightM->begin(), knightM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), pawnM->begin(), pawnM->end());
		//allPossibleMoves->insert(allPossibleMoves->end(), rookM->begin(), rookM->end());
		delete bishopM;
		delete queenM;
		delete kingM;
		delete knightM;
		delete pawnM;
		delete rookM;

		return allPossibleMoves;
	}  

*/

	bool isPieceHere(const int& x, const int& y) {
		int squareIndex = (y * 8) + x;
		if ((bitBoard & (1ULL << squareIndex)) != 0) 	{
			return true;
		}
		return false;
	}

	int evaluateBoardValue() const {
		int whitePiecesValue = 0,
			blackPiecesValue;

		whitePiecesValue += whKingPiece->getNumPieces() * whKingPiece->getValue();
		whitePiecesValue += whPawnPiece->getNumPieces() * whPawnPiece->getValue();
		whitePiecesValue += whRookPiece->getNumPieces() * whRookPiece->getValue();
		whitePiecesValue += whQueenPiece->getNumPieces() * whQueenPiece->getValue();
		whitePiecesValue += whKnightPiece->getNumPieces() * whKnightPiece->getValue();
		whitePiecesValue += whBishopPiece->getNumPieces() * whBishopPiece->getValue();

		blackPiecesValue += blKingPiece->getNumPieces() * blKingPiece->getValue();
		blackPiecesValue += blPawnPiece->getNumPieces() * blPawnPiece->getValue();
		blackPiecesValue += blRookPiece->getNumPieces() * blRookPiece->getValue();
		blackPiecesValue += blQueenPiece->getNumPieces() * blQueenPiece->getValue();
		blackPiecesValue += blKnightPiece->getNumPieces() * blKnightPiece->getValue();
		blackPiecesValue += blBishopPiece->getNumPieces() * blBishopPiece->getValue();

		return whitePiecesValue + blackPiecesValue;
	}
	void makeAIMove(uint16_t& beforeNAfterMove, bool& isItWhiteMove) {
		int newTileIndex = (beforeNAfterMove & 16128) >> 8;
		int initialTileIndex = beforeNAfterMove & 63;
		bool whiteCaptured = false,
			blackCaptured = false;
		//create a stack for captured pieces. When undoing a move, pop from stack. When commited to a move, clear stack.

		if (((1ULL << initialTileIndex) & whKing) != 0) {  //if white king had moved
			timesWhiteKingMoved++;
			whKingPiece->updateHasKingMoved();
		}
		else if (((1ULL << initialTileIndex) & blKing) != 0) { //if black king had moved
			timesBlackKingMoved++;
			blKingPiece->updateHasKingMoved();
		}


		if (isItWhiteMove == true) 	{ //if captured a piece
			if (((1ULL << newTileIndex) & blPieces) != 0) 	{

			}
		}

		if ((beforeNAfterMove & 49344) == 0) { //if no special moves
			
		}
		else if ((beforeNAfterMove & 64) == 64) { //Pawn promotion

			
		}
		else if ((beforeNAfterMove & 128) == 128) { //pawn moved two squares
			if (isItWhiteMove == true) 	{
				whPawnPiece->removeAPieceFromBoard(initialTileIndex);
				whPawnPiece->addAPieceToBoard(newTileIndex);
				whPawnPiece->updateBitBoardPosition();
			}
			else 	{
				blPawnPiece->removeAPieceFromBoard(initialTileIndex);
				blPawnPiece->addAPieceToBoard(newTileIndex);
				whPawnPiece->updateBitBoardPosition();
			}
		}
		else if ((beforeNAfterMove & 16384) == 16384) { //if enpassant is possible
				madeEnPassantMove = true;
			
		}
		else { //castling
			if (isItWhiteMove == true) 	{
				whKingPiece->removeAPieceFromBoard(60);
				if (true) 	{

				}
				whKingPiece->addAPieceToBoard(newTileIndex)
			}
				madeCastleMove = true;
				rookBeforeCastle = (move & 63);
				rookAfterCastle = (move & 16128) >> 8;
			
		}
	}
	void undoAIMove(uint16_t& beforeNAfterMove, bool& isItWhiteMove) {
		int newTileIndex = (beforeNAfterMove & 16128) >> 8;
		int initialTileIndex = beforeNAfterMove & 63;

		if (((1ULL << newTileIndex) & whKing) != 0) {  //if white king had moved
			timesWhiteKingMoved--;
			if (timesWhiteKingMoved == 0)
				whKingPiece->undoHasKingMoved();
		}
		else if (((1ULL << newTileIndex) & blKing) != 0) { //if black king had moved
			timesBlackKingMoved--;
			if (timesBlackKingMoved == 0)
				blKingPiece->undoHasKingMoved();
		}
		
	}

	void addWhiteQueenToBoard(const int& index) {
		whQueen |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addWhiteRookToBoard(const int& index) {
		whRook |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addWhiteBishopToBoard(const int& index) {
		whBishop |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addWhiteKnightToBoard(const int& index) {
		whKnight |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}

	void addBlackQueenToBoard(const int& index) {
		blQueen |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addBlackRookToBoard(const int& index) {
		blRook |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addBlackBishopToBoard(const int& index) {
		blBishop |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addBlackKnightToBoard(const int& index) {
		blKnight |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}

	void removeCapturedPiece(const int& index) {
		bitBoard &= ~(1ULL << index);
		
		whPawn &= bitBoard;
		whRook &= bitBoard;
		whBishop &= bitBoard;
		whKnight &= bitBoard;
		whKing &= bitBoard;
		whQueen &= bitBoard;

		whPieces &= ~(1ULL << index);

		blPawn &= bitBoard;
		blRook &= bitBoard;
		blBishop &= bitBoard;
		blKnight &= bitBoard;
		blKing &= bitBoard;
		blQueen &= bitBoard;

		blPieces &= ~(1ULL << index);

		updateDisplayOfPieces();
	}

	void updateBitBoard() {
		bitBoard = 0ULL;
		bitBoard |= whPawn;
		bitBoard |= whRook;
		bitBoard |= whBishop;
		bitBoard |= whKnight;
		bitBoard |= whKing;
		bitBoard |= whQueen;
		
		whPieces = 0ULL;
		whPieces |= bitBoard;

		bitBoard |= blPawn;
		bitBoard |= blRook;
		bitBoard |= blBishop;
		bitBoard |= blKnight;
		bitBoard |= blKing;
		bitBoard |= blQueen;

		blPieces = 0ULL;
		blPieces |= bitBoard ^ whPieces;
	}
	void updateDisplayOfPieces() {
		whKingPiece->updatePositionForDisplay();
		whPawnPiece->updatePositionForDisplay();
		whRookPiece->updatePositionForDisplay();
		whQueenPiece->updatePositionForDisplay();
		whBishopPiece->updatePositionForDisplay();
		whKnightPiece->updatePositionForDisplay();

		blKingPiece->updatePositionForDisplay();
		blPawnPiece->updatePositionForDisplay();
		blRookPiece->updatePositionForDisplay();
		blQueenPiece->updatePositionForDisplay();
		blBishopPiece->updatePositionForDisplay();
		blKnightPiece->updatePositionForDisplay();
	}
	void updateSquaresWhiteAttacks() {
		squaresToBlockCheckOrCapture = 0ULL;
		locationOfPieceAttackingKing = 0ULL;
		pinnedPiecesBitBoard = 0ULL;
		checkPathXRayThroughKing = 0ULL;
		enemyPiecesThatAreDefended = 0ULL;
		unsigned long long closestPiecesToKing = findClosestPieceToKing(numOfTrailingZeros(blKing)); //for finding pinned pieces for black

		whKingPiece->updateAttackSquares(whKing, whPieces);
		notCapturable ^= blKing;
		whPawnPiece->updateAttackSquaresWhite(whPawn, whPieces);
		whRookPiece->updateAttackSquares(whRook, blKing, closestPiecesToKing, whPieces);
		whQueenPiece->updateAttackSquares(whQueen, blKing, closestPiecesToKing, whPieces);
		whBishopPiece->updateAttackSquares(whBishop, blKing, closestPiecesToKing, whPieces);
		whKnightPiece->updateAttackSquares(whKnight, blKing, whPieces);
		notCapturable = ~(blPieces | whKing);
		
		printBitBoard(whPieces);

		squaresWhiteAttacks = 0ULL;
		squaresWhiteAttacks |= attackSquaresKing;
		squaresWhiteAttacks |= attackSquaresQueen;
		squaresWhiteAttacks |= attackSquaresRook;
		squaresWhiteAttacks |= attackSquaresBishop;
		squaresWhiteAttacks |= attackSquaresPawn;
		squaresWhiteAttacks |= attackSquaresKnight;

		blKingPiece->setSquaresTheEnemyAttacks(squaresWhiteAttacks);

		whAttackKing = 0ULL;
		whAttackRook = 0ULL;
		whAttackPawn = 0ULL;
		whAttackQueen = 0ULL;
		whAttackKnight = 0ULL;
		whAttackBishop = 0ULL;

		whAttackKing |= attackSquaresKing;
		whAttackQueen |= attackSquaresQueen;
		whAttackRook |= attackSquaresRook;
		whAttackBishop |= attackSquaresBishop;
		whAttackPawn |= attackSquaresPawn;
		whAttackKnight |= attackSquaresKnight;

		checkForCheckBlack();
	}
	void updateSquaresBlackAttacks() {
		squaresToBlockCheckOrCapture = 0ULL;
		locationOfPieceAttackingKing = 0ULL;
		pinnedPiecesBitBoard = 0ULL;
		checkPathXRayThroughKing = 0ULL;
		enemyPiecesThatAreDefended = 0ULL;
		unsigned long long closestPiecesToKing = findClosestPieceToKing(numOfTrailingZeros(whKing)); //for finding pinned pieces for white

		blKingPiece->updateAttackSquares(blKing, blPieces);
		notCapturable ^= whKing; //useful for finding checks
		blPawnPiece->updateAttackSquaresBlack(blPawn, blPieces);
		blRookPiece->updateAttackSquares(blRook, whKing, closestPiecesToKing, blPieces);
		blQueenPiece->updateAttackSquares(blQueen, whKing, closestPiecesToKing, blPieces);
		blBishopPiece->updateAttackSquares(blBishop, whKing, closestPiecesToKing, blPieces);
		blKnightPiece->updateAttackSquares(blKnight, whKing, blPieces);
		notCapturable = ~(whPieces | blKing); //For 

		squaresBlackAttacks = 0ULL;
		squaresBlackAttacks |= attackSquaresKing;
		squaresBlackAttacks |= attackSquaresQueen;
		squaresBlackAttacks |= attackSquaresRook;
		squaresBlackAttacks |= attackSquaresBishop;
		squaresBlackAttacks |= attackSquaresPawn;
		squaresBlackAttacks |= attackSquaresKnight;

		whKingPiece->setSquaresTheEnemyAttacks(squaresBlackAttacks);

		blAttackKing = 0ULL;
		blAttackRook = 0ULL;
		blAttackPawn = 0ULL;
		blAttackQueen = 0ULL;
		blAttackKnight = 0ULL;
		blAttackBishop = 0ULL;

		blAttackKing |= attackSquaresKing;
		blAttackQueen |= attackSquaresQueen;
		blAttackRook |= attackSquaresRook;
		blAttackBishop |= attackSquaresBishop;
		blAttackPawn |= attackSquaresPawn;
		blAttackKnight |= attackSquaresKnight;
		
		checkForCheckWhite(); //check if white is now in check
	}
	void updateIsWhiteMove() {
		isWhiteMove = !isWhiteMove;
	}

	void checkForCheckWhite() {
		numberOfChecks = 0;
		
		if ((squaresBlackAttacks & whKing) == 0) {
			squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF;
			if (checkIfWhiteIsStaleMated() == true) 	{
				isStaleMate = true;
			}
			return;
		}

		if ((blAttackBishop & whKing) != 0) { numberOfChecks++; }
		if ((blAttackQueen & whKing) != 0) 	{ numberOfChecks++; }
		if ((blAttackRook & whKing) != 0) { numberOfChecks++; }
		if ((blAttackKnight & whKing) != 0) { numberOfChecks++; }
		if ((blAttackPawn & whKing) != 0) { numberOfChecks++; }

		if (numberOfChecks == 0) { squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF; }
		else if (checkIfWhiteIsMated() == true) { isCheckMate = true; }

	}
	void checkForCheckBlack() {
		numberOfChecks = 0;

		if ((squaresWhiteAttacks & blKing) == 0) {
			squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF;
			if (checkIfBlackIsStaleMated() == true) 	{
				isStaleMate = true;
			}
			return;
		}

		if ((whAttackBishop & blKing) != 0) { numberOfChecks++;	}
		if ((whAttackQueen & blKing) != 0) { numberOfChecks++; }
		if ((whAttackRook & blKing) != 0) { numberOfChecks++; }
		if ((whAttackKnight & blKing) != 0) { numberOfChecks++; }
		if ((whAttackPawn & blKing) != 0) { numberOfChecks++; }

		if (numberOfChecks == 0) { squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF; }
		else if (checkIfBlackIsMated() == true) { isCheckMate = true; }
	}
	
	bool checkIfWhiteIsMated() { return checkIfWhiteCannotMove(); }
	bool checkIfBlackIsMated() { return checkIfBlackCannotMove(); }
	bool checkIfWhiteIsStaleMated() { return checkIfWhiteCannotMove(); }
	bool checkIfBlackIsStaleMated() { return checkIfBlackCannotMove(); }
	bool checkIfWhiteCannotMove() {
		if (queenMovesWhite()->size() == 0)
			if (rookMovesWhite()->size() == 0)
				if (bishopMovesWhite()->size() == 0)
					if (knightMovesWhite()->size() == 0)
						if (pawnMovesWhite()->size() == 0)
							if (enPassantMovesWhite()->size() == 0)
								if (kingMovesWhite()->size() == 0)
									return true;
		return false;
	}
	bool checkIfBlackCannotMove() {
		if (queenMovesBlack()->size() == 0)
			if (rookMovesBlack()->size() == 0)
				if (bishopMovesBlack()->size() == 0)
					if (knightMovesBlack()->size() == 0)
						if (pawnMovesBlack()->size() == 0)
							if (enPassantMovesBlack()->size() == 0)
								if (kingMovesBlack()->size() == 0)
									return true;
		return false;
	}

	unsigned long long findClosestPieceToKing(const int& kingLocation) {
		return (HorzNVerticalMoves(kingLocation) | diagNAntiDagMoves(kingLocation) & notCapturable);
	}

	bool getIsWhiteMove() const { return isWhiteMove; }
	bool getIsCheckMate() const { return isCheckMate; }
	bool getIsStaleMate() const { return isStaleMate; }

	Pawn* getWhitePawnPiece() { return whPawnPiece; }
	Rook* getWhiteRookPiece() { return whRookPiece; }
	Bishop* getWhiteBishopPiece() { return whBishopPiece; }
	Knight* getWhiteKnightPiece() { return whKnightPiece; }
	Queen* getWhiteQueenPiece() { return whQueenPiece; }
	King* getWhiteKingPiece() { return whKingPiece; }

	Pawn* getBlackPawnPiece() { return blPawnPiece; }
	Rook* getBlackRookPiece() { return blRookPiece; }
	Bishop* getBlackBishopPiece() { return blBishopPiece; }
	Knight* getBlackKnightPiece() { return blKnightPiece; }
	Queen* getBlackQueenPiece() { return blQueenPiece; }
	King* getBlackKingPiece() { return blKingPiece; }

	std::unique_ptr<std::vector<uint16_t>> kingMovesWhite() { return whKingPiece->legalMoves(whKing); }
	std::unique_ptr<std::vector<uint16_t>> queenMovesWhite() { return whQueenPiece->legalMoves(whQueen); }
	std::unique_ptr<std::vector<uint16_t>> rookMovesWhite() { return whRookPiece->legalMoves(whRook); }
	std::unique_ptr<std::vector<uint16_t>> bishopMovesWhite() { return whBishopPiece->legalMoves(whBishop); }
	std::unique_ptr<std::vector<uint16_t>> knightMovesWhite() { return whKnightPiece->legalMoves(whKnight); }
	std::unique_ptr<std::vector<uint16_t>> pawnMovesWhite() { return whPawnPiece->pawnMovesWhite(whPawn); }
	std::unique_ptr<std::vector<uint16_t>> castlingMovesWhite() { return whKingPiece->castlingMovesWhite(); }
	std::unique_ptr<std::vector<uint16_t>> enPassantMovesWhite() { return whPawnPiece->ePassantSquaresWhite(whPawn); }

	std::unique_ptr<std::vector<uint16_t>> kingMovesBlack() { return blKingPiece->legalMoves(blKing); }
	std::unique_ptr<std::vector<uint16_t>> queenMovesBlack() { return blQueenPiece->legalMoves(blQueen); }
	std::unique_ptr<std::vector<uint16_t>> rookMovesBlack() { return blRookPiece->legalMoves(blRook); }
	std::unique_ptr<std::vector<uint16_t>> bishopMovesBlack() { return blBishopPiece->legalMoves(blBishop); }
	std::unique_ptr<std::vector<uint16_t>> knightMovesBlack() { return blKnightPiece->legalMoves(blKnight); }
	std::unique_ptr<std::vector<uint16_t>> pawnMovesBlack() { return blPawnPiece->pawnMovesBlack(blPawn); }
	std::unique_ptr<std::vector<uint16_t>> castlingMovesBlack() { return blKingPiece->castlingMovesBlack(); }
	std::unique_ptr<std::vector<uint16_t>> enPassantMovesBlack() { return blPawnPiece->ePassantSquaresBlack(blPawn); }
};

#endif
	/*
	void testMoveGenerationSpeed() {

		notCapturable = ~(whPieces | blKing); //avoid illegal capture of black king
		std::unique_ptr<std::vector<uint16_t>> bishopM, queenM, kingM, knightM, pawnM, rookM, castleM, enPass;
		attackedSquares = 0ULL;

		bishopM = bishopMovesWhite();
		queenM = queenMovesWhite();
		kingM = kingMovesWhite();
		knightM = knightMovesWhite();
		pawnM = pawnMovesWhite();
		rookM = rookMovesWhite();
		enPass = enPassantMovesWhite();
		updateSquaresWhiteAttacks();
	}
	*/