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

class ChessBoard : public BoardInfo{
protected:
	static const int BSIZE = 8;
	float tileWidth;
	bool isWhiteMove; //true for white move, false for black
	bool isCheckMate = false,
		isStaleMate = false;
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

public:											  
												  
	ChessBoard(){
		isWhiteMove = true;
		tileWidth = 0;
	}								  
												  
	ChessBoard(const float &tileWidth, sf::Color & whiteCol, sf::Color & blkCol, const float &frstSquareCentRef) {
		this->tileWidth = tileWidth;
		whtTileColor = whiteCol;
		blkTileColor = blkCol;
		isWhiteMove = true;
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
		//                            Value, Image Scale,       img path,                x cord,         y cord    whitePiece
		//Bishop whBishopPiece(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef + (tileWidth * 5), whiteYOffset, true);
		//Knight whKnightPiece(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef + (tileWidth * 6), whiteYOffset, true);
		//Rook whRookPiece(rookVal, pieceScale, "assets/white-rook.png", whiteYOffset, whiteYOffset, true);
		//whRookPiece.setBitBoardPosition(whRook);
		//whKnightPiece.setBitBoardPosition(whKnight);
		//whBishopPiece.setBitBoardPosition(whBishop);
		//whQueenPiece.setBitBoardPosition(whQueen);
		//whKingPiece.setBitBoardPosition(whKing);
		//whPawnPiece.setBitBoardPosition(whPawn);

		//Black pieces
		//Bishop(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef + (tileWidth * 5), frstSquareCentRef, false);
		//Knight(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef + (tileWidth * 6), frstSquareCentRef, false);
		//Rook(rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef + (tileWidth * 7), frstSquareCentRef, false);
		//blRookPiece.setBitBoardPosition(blRook);
		//blKnightPiece.setBitBoardPosition(blKnight);
		//blBishopPiece.setBitBoardPosition(blBishop);
		//blQueenPiece.setBitBoardPosition(blQueen);
		//blKingPiece.setBitBoardPosition(blKing);

		/*
		pieceHolder[0] = new Rook (rookVal, pieceScale, "assets/white-rook.png", frstSquareCentRef, true, "R", whRook);
		pieceHolder[1] = new Knight (knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef, true, "N", whKnight);
		pieceHolder[2] = new Bishop (bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef, true, "B", whBishop);
		pieceHolder[3] = new Queen (queenVal, pieceScale, "assets/white-queen.png", frstSquareCentRef, true, "Q", whQueen);
		pieceHolder[4] = new King (kingVal, pieceScale, "assets/white-king.png", frstSquareCentRef, true, "K", whKing);
		pieceHolder[5] = new Pawn (pawnVal, pieceScale, "assets/white-pawn.png", frstSquareCentRef, true, "P", whPawn);
					
		pieceHolder[6] = new Rook (rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef, false, "r", blRook);
		pieceHolder[7] = new Knight (knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef, false, "n", blKnight);
		pieceHolder[8] = new Bishop (bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef, false, "b", blBishop);
		pieceHolder[9] = new Queen (queenVal * -1, pieceScale, "assets/black-queen.png", frstSquareCentRef, false, "q", blQueen);
		pieceHolder[10] = new King (kingVal * -1, pieceScale, "assets/black-king.png", frstSquareCentRef, false, "k", blKing);
		pieceHolder[11] = new Pawn (pawnVal * -1, pieceScale, "assets/black-pawn.png", frstSquareCentRef, false, "p", blPawn);
		*/
		
		//blPawnPiece.setBitBoardPosition(blPawn);

		/*
		Rook   whRookPiece  (rookVal, pieceScale, "assets/white-rook.png", frstSquareCentRef, true, "R", whRook);
		Knight whKnightPiece(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef, true, "N", whKnight);
		Bishop whBishopPiece(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef, true, "B", whBishop);
		Queen  whQueenPiece (queenVal, pieceScale, "assets/white-queen.png", frstSquareCentRef, true, "Q", whQueen);
		King   whKingPiece  (kingVal, pieceScale, "assets/white-king.png", frstSquareCentRef, true, "K", whKing);
		Pawn   whPawnPiece  (pawnVal, pieceScale, "assets/white-pawn.png", frstSquareCentRef, true, "P", whPawn);

		Rook   blRookPiece  (rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef, false, "r", blRook);
		Knight blKnightPiece(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef, false, "n", blKnight);
		Bishop blBishopPiece(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef, false, "b", blBishop);
		Queen  blQueenPiece (queenVal * -1, pieceScale, "assets/black-queen.png", frstSquareCentRef, false, "q", blQueen);
		King   blKingPiece  (kingVal * -1, pieceScale, "assets/black-king.png", frstSquareCentRef, false, "k", blKing);
		Pawn   blPawnPiece  (pawnVal * -1, pieceScale, "assets/black-pawn.png", frstSquareCentRef, false, "p", blPawn);
		*/
		
		whRookPiece = new Rook(rookVal, pieceScale, "assets/white-rook.png", frstSquareCentRef, true, "R", whRook);
		whKnightPiece = new Knight(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef, true, "N", whKnight);
		whBishopPiece = new Bishop(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef, true, "B", whBishop);
		whQueenPiece = new Queen(queenVal, pieceScale, "assets/white-queen.png", frstSquareCentRef, true, "Q", whQueen);
		whKingPiece = new King(kingVal, pieceScale, "assets/white-king.png", frstSquareCentRef, true, "K", whKing);
		whPawnPiece = new Pawn(pawnVal, pieceScale, "assets/white-pawn.png", frstSquareCentRef, true, "P", whPawn);

		blRookPiece = new Rook(rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef, false, "r", blRook);
		blKnightPiece = new Knight(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef, false, "n", blKnight);
		blBishopPiece = new Bishop(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef, false, "b", blBishop);
		blQueenPiece = new Queen(queenVal * -1, pieceScale, "assets/black-queen.png", frstSquareCentRef, false, "q", blQueen);
		blKingPiece = new King(kingVal * -1, pieceScale, "assets/black-king.png", frstSquareCentRef, false, "k", blKing);
		blPawnPiece = new Pawn(pawnVal * -1, pieceScale, "assets/black-pawn.png", frstSquareCentRef, false, "p", blPawn);
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
		whRookPiece->printSprite(window);
		whQueenPiece->printSprite(window);
		whKingPiece->printSprite(window);
		whPawnPiece->printSprite(window);
		whKnightPiece->printSprite(window);
		whBishopPiece->printSprite(window);

		blRookPiece->printSprite(window);
		blKnightPiece->printSprite(window);
		blBishopPiece->printSprite(window);
		blQueenPiece->printSprite(window);
		blKingPiece->printSprite(window);
		blPawnPiece->printSprite(window);
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
	int testOfTrailingZeros(unsigned long long bitMap) {
		
		//doesn't work
		if (bitMap & 0x1) 	{
			return 0;
		}
		int d = 1;
		d += 2 * !(bitMap >> (4 * !(bitMap >> (8 * !(bitMap >> (16 * !(bitMap >> (32 * !(bitMap & 0xffffffff)) & 0xffff)) & 0xff)) & 0xf)) & 0x3);
		d -= bitMap & 0x1;

		//std::cout << d << std::endl;
		return d;


		//try two
		int resultVal = 0;
		int d = 1;

		resultVal = 32 * !(bitMap & 0xffffffff);
		bitMap >>= resultVal;
		d += resultVal;

		resultVal = 16 * !(bitMap & 0xffff);
		bitMap >>= resultVal;
		d += resultVal;

		resultVal = 8 * !(bitMap & 0xff);
		bitMap >>= resultVal;
		d += resultVal;

		resultVal = 4 * !(bitMap & 0xf);
		bitMap >>= resultVal;
		d += resultVal;

		resultVal = 2 * !(bitMap & 0x3);
		bitMap >>= resultVal;
		d += resultVal;

		d -= bitMap & 0x1;










	}
	*/
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

	void addWhiteQueenToBoard(const int& index) {
		whQueen |= (1ULL << index);
		bitBoard |= (1ULL << index);
	}
	void addWhiteRookToBoard(const int& index) {
		whRook |= (1ULL << index);
		bitBoard |= (1ULL << index);
		//whRookPiece->updatePositionForDisplay();
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
		castleRooks;
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
	void updateAllBitboards() { //allows the AI to update the bitboards to new or previous iterations.
		
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

	unsigned long long findClosestPieceToKing(const int& kingLocation) {
		return (HorzNVerticalMoves(kingLocation) | diagNAntiDagMoves(kingLocation) & notCapturable);
	}

	bool getIsWhiteMove() const {
		return isWhiteMove;
	}
	bool getIsCheckMate() const { 
		return isCheckMate; }
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
	/*
	void movePiece(const int& x, const int& y, sf::RenderWindow& window) {
		bool grabbedPiece = true;
		int newX, newY;
		bool isLegalMove = false;

		if (isWhiteMove) { //white to move
			if (!pieceBoard[x][y]->getIsWhite()) { //if piece is not white
				return;
			}
		}
		else { //black turn to move
			if (pieceBoard[x][y]->getIsWhite()) { //if piece is white
				return;
			}
		}

		sf::Sprite *pieceSprite= &pieceBoard[x][y]->getSprite();

		while (grabbedPiece) {
			sf::Event e;
			while (window.pollEvent(e)) {
				
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				pieceSprite->setPosition(sf::Vector2f((float)mousePos.x, (float)mousePos.y));

				if (e.type == sf::Event::MouseButtonReleased) {

					//finalizeMove(e, mousePos, pieceSprite, x, y);
				
					newX = (mousePos.x) / 120;
					newY = (mousePos.y) / 120;

					pieceSprite->setPosition(sf::Vector2f((float)(newX * 120 + 60), (float)(newY * 120 + 60)));



					if (newX != x || newY != y) { 
						isLegalMove = pieceBoard[x][y]->legalMove(pieceBoard, x, y, newX, newY);
						
						if (!pieceBoard[newX][newY]) { //if no piece on tile
							if (pieceBoard[x][y]->getDidSpecialMove()) { //special moves are done internally for the piece
								return;
							}
							if (isLegalMove) {
								pieceBoard[newX][newY] = pieceBoard[x][y];
								pieceBoard[x][y] = NULL;
							}
							else {
								pieceSprite->setPosition(sf::Vector2f((float)(x * 120 + 60), (float)(y * 120 + 60)));
								return;
							}
						}
						else if (pieceBoard[x][y]->getIsWhite() == pieceBoard[newX][newY]->getIsWhite()) { //if same color on tile
							pieceSprite->setPosition(sf::Vector2f((float)(x * 120 + 60), (float)(y * 120 + 60)));
							return;
						}
						else if (isLegalMove) {//capture
							pieceBoard[newX][newY] = pieceBoard[x][y];
							pieceBoard[x][y] = NULL;
						}
						else {
							pieceSprite->setPosition(sf::Vector2f((float)(x * 120 + 60), (float)(y * 120 + 60)));
							return;
						}


					}
					
					grabbedPiece = false;
				
				}

				window.clear();
				drawBoard(window);
				window.display();
			}
		}
		

	}




	void findPieceType(int value) {

	}

	void finalizeMove(sf::Event& e, sf::Vector2i& mousePos, sf::Sprite* pieceSprite, const int &x, const int& y) {
		bool grabbedPiece = true;
		int newX, newY;
		bool isLegalMove = false;

		newX = (mousePos.x) / 120;
		newY = (mousePos.y) / 120;

		pieceSprite->setPosition(sf::Vector2f((float)(newX * 120 + 60), (float)(newY * 120 + 60)));



		if (val < 0) {
			val *= -1;
		}



























		if (newX != x || newY != y) {
			isLegalMove = pieceBoard[x][y]->legalMove(pieceBoard, x, y, newX, newY);

			if (!pieceBoard[newX][newY]) { //if no piece on tile
				if (pieceBoard[x][y]->getDidSpecialMove()) { //special moves are done internally for the piece
					return;
				}
				if (isLegalMove) {
					pieceBoard[newX][newY] = pieceBoard[x][y];
					pieceBoard[x][y] = NULL;
				}
				else {
					pieceSprite->setPosition(sf::Vector2f((float)(x * 120 + 60), (float)(y * 120 + 60)));
					return;
				}
			}
			else if (pieceBoard[x][y]->getIsWhite() == pieceBoard[newX][newY]->getIsWhite()) { //if same color on tile
				pieceSprite->setPosition(sf::Vector2f((float)(x * 120 + 60), (float)(y * 120 + 60)));
				return;
			}
			else if (isLegalMove) {//capture
				pieceBoard[newX][newY] = pieceBoard[x][y];
				pieceBoard[x][y] = NULL;
			}
			else {
				pieceSprite->setPosition(sf::Vector2f((float)(x * 120 + 60), (float)(y * 120 + 60)));
				return;
			}


		}

		grabbedPiece = false;












	}
	*/
};

#endif

