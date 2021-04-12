#ifndef PAWN_H
#define PAWN_H //test
#include "Piece.h"
#include <string>

class Pawn : public Piece {


public:
	Pawn() {

	}
	Pawn(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition) {
	}
	void drawPiece(sf::RenderWindow& window) {
		Piece::drawPiece(window);
	}

	void findMoveableSquares(const int& piecePosition) {
		if (isWhite)
			singlePieceMoveableSquaresWhite(piecePosition);
		else
			singlePieceMoveableSquaresBlack(piecePosition);
	}

	void singlePieceMoveableSquaresWhite(const int& piecePosition) {
		unsigned long long pieceBitBoard = (1ULL << piecePosition);
		movableSquaresForDisplay = 0ULL;
		if ((pieceBitBoard & pinnedPiecesBitBoard) != 0) 	{ //if pawn is pinned
			if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & whKing) != 0) //if king is at a diagonal
				movableSquaresForDisplay |= (pieceBitBoard >> 7) & bitBoard & notCapturable & ~FILE_A; //Capture right. Includes potential promotion
			else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & whKing) != 0) //if king is at antiDiagonal
				movableSquaresForDisplay |= (pieceBitBoard >> 9) & bitBoard & notCapturable & ~FILE_H; //Capture left. Includes potential promotion
			else if ((rankMasks[piecePosition / 8] & whKing) != 0) //if king is on same rank (horz)
				return;
			else if ((fileMasks[piecePosition % 8] & whKing) != 0) { //if king is on same file (vert)
				movableSquaresForDisplay |= (pieceBitBoard >> 8) & ~bitBoard & notCapturable;
				movableSquaresForDisplay |= (pieceBitBoard >> 16) & ~bitBoard & ~(bitBoard >> 8) & RANK_4 & notCapturable;
			}
			else
				std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
			return;
		}
		movableSquaresForDisplay |= (pieceBitBoard >> 7) & bitBoard & notCapturable & ~FILE_A; //Capture right. Includes potential promotion
		movableSquaresForDisplay |= (pieceBitBoard >> 9) & bitBoard & notCapturable & ~FILE_H; //Capture left. Includes potential promotion
		movableSquaresForDisplay |= (pieceBitBoard >> 7) & enPassantWhite; //enPassant right
		movableSquaresForDisplay |= (pieceBitBoard >> 9) & enPassantWhite; //enPassant left

		if ((rankMasks[piecePosition / 8] & whKing) != 0) { //edge case to prevent pawn from enPassanting their king into check
			if ((rankMasks[piecePosition / 8] & blRook) != 0 || (rankMasks[piecePosition / 8] & blQueen) != 0) {
				movableSquaresForDisplay ^= (pieceBitBoard >> 7) & bitBoard & notCapturable & ~FILE_A;
				movableSquaresForDisplay ^= (pieceBitBoard >> 9) & bitBoard & notCapturable & ~FILE_H;
				std::cout << "Special Condition found" << std::endl;
			}
		}

		movableSquaresForDisplay |= (pieceBitBoard >> 8) & ~bitBoard;
		movableSquaresForDisplay |= (pieceBitBoard >> 16) & ~bitBoard & ~(bitBoard >> 8)& RANK_4;
		movableSquaresForDisplay &= squaresToBlockCheckOrCapture;
	}
	void singlePieceMoveableSquaresBlack(const int& piecePosition) {
		unsigned long long pieceBitBoard = (1ULL << piecePosition);
		movableSquaresForDisplay = 0ULL;
		if ((pieceBitBoard & pinnedPiecesBitBoard) != 0) { //if pawn is pinned
			if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & blKing) != 0) //if king is at a diagonal
				movableSquaresForDisplay |= (pieceBitBoard << 7) & bitBoard & notCapturable & ~FILE_H; //Capture right. Includes potential promotion
			else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & blKing) != 0) //if king is at antiDiagonal
				movableSquaresForDisplay |= (pieceBitBoard << 9) & bitBoard & notCapturable & ~FILE_A; //Capture left. Includes potential promotion
			else if ((rankMasks[piecePosition / 8] & blKing) != 0) //if king is on same rank (horz)
				return;
			else if ((fileMasks[piecePosition % 8] & blKing) != 0) { //if king is on same file (vert)
				movableSquaresForDisplay |= (pieceBitBoard << 8) & ~bitBoard;
				movableSquaresForDisplay |= (pieceBitBoard << 16) & ~bitBoard & ~(bitBoard << 8) & RANK_5;
			}
			else
				std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
			return;
		}
		movableSquaresForDisplay |= (pieceBitBoard << 7) & bitBoard & notCapturable & ~FILE_H; //Capture right. Includes potential promotion
		movableSquaresForDisplay |= (pieceBitBoard << 9) & bitBoard & notCapturable & ~FILE_A; //Capture left. Includes potential promotion
		movableSquaresForDisplay |= (pieceBitBoard << 7) & enPassantBlack; //enPassant right
		movableSquaresForDisplay |= (pieceBitBoard << 9) & enPassantBlack; //enPassant left
		
		if ((rankMasks[piecePosition / 8] & blKing) != 0 ) 	{ //edge case to prevent pawn from enPassanting their king into check
			if ((rankMasks[piecePosition / 8] & whRook) != 0 || (rankMasks[piecePosition / 8] & whQueen) != 0) 	{
				movableSquaresForDisplay ^= (pieceBitBoard << 7) & bitBoard & notCapturable & ~FILE_H;
				movableSquaresForDisplay ^= (pieceBitBoard << 9) & bitBoard & notCapturable & ~FILE_A;
				std::cout << "Special Condition found" << std::endl;
			}
		}
		
		movableSquaresForDisplay |= (pieceBitBoard << 8) & ~bitBoard;
		movableSquaresForDisplay |= (pieceBitBoard << 16) & ~bitBoard & ~(bitBoard << 8) & RANK_5;
		movableSquaresForDisplay &= squaresToBlockCheckOrCapture;
	}

	void updateAttackSquaresWhite(const unsigned long long& pieceBitBoard, const unsigned long long& myPieces) {
		attackSquaresPawn = 0ULL;
		attackSquaresPawn |= (pieceBitBoard >> 7) & notCapturable & ~FILE_A; //Capture right. Includes potential promotion
		attackSquaresPawn |= (pieceBitBoard >> 9) & notCapturable & ~FILE_H; //Capture left. Includes potential promotion

		enemyPiecesThatAreDefended |= (pieceBitBoard >> 7) & myPieces & ~FILE_A;
		enemyPiecesThatAreDefended |= (pieceBitBoard >> 9) & myPieces & ~FILE_H;
	}
	void updateAttackSquaresBlack(const unsigned long long& pieceBitBoard, const unsigned long long& myPieces) {
		attackSquaresPawn = 0ULL;
		attackSquaresPawn |= (pieceBitBoard << 7) & notCapturable & ~FILE_H; //Capture right. Includes potential promotion
		attackSquaresPawn |= (pieceBitBoard << 9) & notCapturable & ~FILE_A; //Capture left. Includes potential promotion

		enemyPiecesThatAreDefended |= (pieceBitBoard << 7) & myPieces & ~FILE_H;
		enemyPiecesThatAreDefended |= (pieceBitBoard << 9) & myPieces & ~FILE_A;
	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() {
		std::unique_ptr<std::vector<uint16_t>> allPossibleMoves = std::make_unique<std::vector<uint16_t>>(),
			standardMoves = std::make_unique<std::vector<uint16_t>>(),
			enPassantMoves = std::make_unique<std::vector<uint16_t>>();


		if (isWhite) 	{
			standardMoves = pawnMovesWhite(whPawn);
			enPassantMoves = ePassantSquaresWhite(whPawn);
		}
		else 	{
			standardMoves = pawnMovesBlack(blPawn);
			enPassantMoves = ePassantSquaresBlack(blPawn);
		}

		allPossibleMoves->insert(allPossibleMoves->end(), standardMoves->begin(), standardMoves->end());
		allPossibleMoves->insert(allPossibleMoves->end(), enPassantMoves->begin(), enPassantMoves->end());

		return allPossibleMoves;
	}

	std::unique_ptr<std::vector<uint16_t>> pawnMovesWhite(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;
		//attackSquaresPawn = 0ULL;
		unsigned long long PAWN_MOVES = (pieceBitBoard >> 7) & notCapturable & bitBoard & ~FILE_A & ~RANK_8 & squaresToBlockCheckOrCapture; //Capture right. Includes potential promotion
		//attackSquaresPawn |= (pieceBitBoard >> 7) & notCapturable & ~FILE_A & ~RANK_8;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare + 7); //Origional position of pawn
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard >> 9) & notCapturable & bitBoard & ~FILE_H & ~RANK_8 & squaresToBlockCheckOrCapture; //Capture left. Includes potential promotion
		//attackSquaresPawn |= (pieceBitBoard >> 9) & notCapturable & ~FILE_H & ~RANK_8;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare + 9);
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard >> 8) & ~bitBoard & ~RANK_8 & squaresToBlockCheckOrCapture; //Move one forward

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare + 8);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		PAWN_MOVES = (pieceBitBoard >> 16) & ~bitBoard & ~(bitBoard >> 8) & RANK_4 & squaresToBlockCheckOrCapture; //Move two forward

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare + 16);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 128; //indicator bit of two square pawn push
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		/***********Potential Pawn Promotion Section**************/

		PAWN_MOVES = (pieceBitBoard >> 7) & notCapturable & bitBoard & ~FILE_A & RANK_8 & squaresToBlockCheckOrCapture; //Capture right. Includes potential promotion
		//attackSquaresPawn |= (pieceBitBoard >> 7) & notCapturable & ~FILE_A & RANK_8;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare + 7);
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			mergeOfBeforeNAfterMove |= 64;
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard >> 9) & notCapturable & bitBoard & ~FILE_H & RANK_8 & squaresToBlockCheckOrCapture; //Capture left. Includes potential promotion
		//attackSquaresPawn |= (pieceBitBoard >> 9) & notCapturable & ~FILE_H & RANK_8;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare + 9);
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			mergeOfBeforeNAfterMove |= 64;
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard >> 8) & ~bitBoard & RANK_8 & squaresToBlockCheckOrCapture; //Move one forward with promotion

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare + 8);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 64;
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		return possibleMoves;
	}
	std::unique_ptr<std::vector<uint16_t>> pawnMovesBlack(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;
		//attackSquaresPawn = 0ULL;
		unsigned long long PAWN_MOVES = (pieceBitBoard << 7) & notCapturable & bitBoard & ~FILE_H & ~RANK_1 & squaresToBlockCheckOrCapture; //Capture right
		//attackSquaresPawn |= (pieceBitBoard << 7) & notCapturable & ~FILE_H & ~RANK_1;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare - 7); //Origional position of pawn
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard << 9) & notCapturable & bitBoard & ~FILE_A & ~RANK_1 & squaresToBlockCheckOrCapture; //Capture left
		//attackSquaresPawn |= (pieceBitBoard << 9) & notCapturable & ~FILE_A & ~RANK_1;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare - 9);
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard << 8) & ~bitBoard & ~RANK_1 & squaresToBlockCheckOrCapture; //Move one forward

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare - 8);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		PAWN_MOVES = (pieceBitBoard << 16) & ~bitBoard & ~(bitBoard << 8) & RANK_5 & squaresToBlockCheckOrCapture; //Move two forward

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare - 16);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 128; //indicator bit of two square pawn push
			possibleMoves->push_back(mergeOfBeforeNAfterMove);
			//enPassantWhite |= (1ULL << (movedSquare - 8));

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		/***********Potential Pawn Promotion Section**************/

		PAWN_MOVES = (pieceBitBoard << 7) & notCapturable & bitBoard & ~FILE_H & RANK_1 & squaresToBlockCheckOrCapture; //Capture right with promotion
		//attackSquaresPawn |= (pieceBitBoard << 7) & notCapturable & ~FILE_H & RANK_1;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare - 7);
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			mergeOfBeforeNAfterMove |= 64; //indicator bit
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard << 9) & notCapturable & bitBoard & ~FILE_A & RANK_1 & squaresToBlockCheckOrCapture; //Capture left with promotion
		//attackSquaresPawn |= (pieceBitBoard << 9) & notCapturable & ~FILE_A & RANK_1;

		while (PAWN_MOVES != 0) {
			int attackedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (attackedSquare - 9);
			mergeOfBeforeNAfterMove |= (attackedSquare << 8);
			mergeOfBeforeNAfterMove |= 64; //indicator bit
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << attackedSquare);
		}

		PAWN_MOVES = (pieceBitBoard << 8) & ~bitBoard & RANK_1 & squaresToBlockCheckOrCapture; //Move one forward to promotion

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare - 8);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 64; //indicator bit
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		return possibleMoves;
	}

	std::unique_ptr<std::vector<uint16_t>> ePassantSquaresWhite(uint64_t pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove; //Before move and after move location. Enemy pawn taken can be found by adding 8 to the final location
		//attackSquaresEnPassant = 0ULL;
		unsigned long long PAWN_MOVES = (pieceBitBoard >> 7) & enPassantWhite & squaresToBlockCheckOrCapture; //enPassant right
		//attackSquaresEnPassant |= PAWN_MOVES;

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare + 7);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 16384; //indicator bit of enPassant
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		PAWN_MOVES = (pieceBitBoard >> 9) & enPassantWhite & squaresToBlockCheckOrCapture; //enPassant left
		//attackSquaresEnPassant |= PAWN_MOVES;

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare + 9);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 16384; //indicator bit of enPassant
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		//unsigned long long PAWN_MOVES = (pieceBitBoard << 7) & notCapturable & bitBoard & ~FILE_H; //Capture right. Includes potential promotion
		return possibleMoves;
	}
	std::unique_ptr<std::vector<uint16_t>> ePassantSquaresBlack(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove; //Before move and after move location. Enemy pawn taken can be found by substracting 8 to the final location
		//attackSquaresEnPassant = 0ULL;
		unsigned long long PAWN_MOVES = (pieceBitBoard << 7) & enPassantBlack & squaresToBlockCheckOrCapture; //enPassant right
		//attackSquaresEnPassant |= PAWN_MOVES;

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare - 7);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 16384; //indicator bit of enPassant
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		PAWN_MOVES = (pieceBitBoard << 9) & enPassantBlack & squaresToBlockCheckOrCapture; //enPassant left
		//attackSquaresEnPassant |= PAWN_MOVES;

		while (PAWN_MOVES != 0) {
			int movedSquare = numOfTrailingZeros(PAWN_MOVES);
			mergeOfBeforeNAfterMove = (movedSquare - 9);
			mergeOfBeforeNAfterMove |= (movedSquare << 8);
			mergeOfBeforeNAfterMove |= 16384; //indicator bit of enPassant
			possibleMoves->push_back(mergeOfBeforeNAfterMove);

			PAWN_MOVES &= ~(1ULL << movedSquare);
		}

		return possibleMoves;
	}
};

#endif