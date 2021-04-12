#ifndef KNIGHT_H
#define KNIGHT_H
#include "Piece.h"
#include <string>

class Knight : public Piece {
private:
	bool hasNotMoved = true;

public:
	Knight() {

	}
	Knight(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition) {
	}
	void drawPiece(sf::RenderWindow& window) {
		Piece::drawPiece(window);
	}

	void findMoveableSquares(const int& piecePosition) {
		singlePieceMoveableSquares(piecePosition);
	}
	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if ((pinnedPiecesBitBoard & (1ULL << piecePosition)) != 0) //A pinned knight cannot move
			return;

		if (piecePosition > 18)
			movableSquaresForDisplay |= KnightSpan << (piecePosition - 18);
		else
			movableSquaresForDisplay |= KnightSpan >> (18 - piecePosition);

		if (piecePosition % 8 < 4)  //prevent magical looping of knight to other side of board
			movableSquaresForDisplay &= ~FILE_GH & notCapturable & squaresToBlockCheckOrCapture;
		else
			movableSquaresForDisplay &= ~FILE_AB & notCapturable & squaresToBlockCheckOrCapture;
	}

	void updateAttackSquares(unsigned long long pieceBitBoard, const unsigned long long& kingBitBoard, const unsigned long long& myPieces) {

		unsigned long long knightPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;

		attackSquaresKnight = 0ULL;
		while (knightPiece != 0) { //find center of knight
			int knightLocation = numOfTrailingZeros(knightPiece);

			if (knightLocation > 18)
				allPotentialMoves = KnightSpan << (knightLocation - 18);
			else
				allPotentialMoves = KnightSpan >> (18 - knightLocation);

			if (knightLocation % 8 < 4)  //prevent magical looping of knight to other side of board
				allPotentialMoves &= ~FILE_GH & notCapturable;
			else
				allPotentialMoves &= ~FILE_AB & notCapturable;

			if ((allPotentialMoves & kingBitBoard) != 0) //knight attack cannot be blocked. Only move away or capture
				squaresToBlockCheckOrCapture |= 1ULL << knightLocation;

			attackSquaresKnight |= allPotentialMoves;
			enemyPiecesThatAreDefended |= allPotentialMoves & myPieces;

			pieceBitBoard &= ~(1ULL << knightLocation);
			knightPiece = pieceBitBoard & ~(pieceBitBoard - 1);
		}

	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite) {
			return legalMoves(whKnight);
		}
		else {
			return legalMoves(blKnight);
		}
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;

		unsigned long long knightPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;
		//attackSquaresKnight = 0ULL;
		while (knightPiece != 0) { //find center of knight
			int knightLocation = numOfTrailingZeros(knightPiece);
			if (knightLocation > 18)
				allPotentialMoves = KnightSpan << (knightLocation - 18);
			else
				allPotentialMoves = KnightSpan >> (18 - knightLocation);

			if (knightLocation % 8 < 4)  //prevent magical looping of knight to other side of board
				allPotentialMoves &= ~FILE_GH & notCapturable & squaresToBlockCheckOrCapture;
			else
				allPotentialMoves &= ~FILE_AB & notCapturable & squaresToBlockCheckOrCapture;

			//attackSquaresKnight |= allPotentialMoves;
			unsigned long long aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			while (aPotentialMove != 0) {
				int movedSquare = numOfTrailingZeros(aPotentialMove);
				mergeOfBeforeNAfterMove = knightLocation;
				mergeOfBeforeNAfterMove |= (movedSquare << 8);
				possibleMoves->push_back(mergeOfBeforeNAfterMove);

				allPotentialMoves &= ~(aPotentialMove);
				aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			}
			pieceBitBoard &= ~(1ULL << knightLocation);
			knightPiece = pieceBitBoard & ~(pieceBitBoard - 1);
		}
		
		return possibleMoves;
	}
};

#endif