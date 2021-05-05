#ifndef ROOK_H
#define ROOK_H
#include "Piece.h"
#include "SlidingPiecesMoves.h"
#include <string>

class Rook : public Piece, private SlidingPiecesMoves {
private:
	

public:
	Rook() {
		
	}
	Rook(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition, const int numPieces)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition, numPieces) {
	}
	void drawPiece(sf::RenderWindow& window) {
		Piece::drawPiece(window);
	}
	
	void findMoveableSquares(const int& piecePosition) { singlePieceMoveableSquares(piecePosition); }
	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if (((1ULL << piecePosition) & pinnedPiecesBitBoard) != 0)//if piece is pinned
			movableSquaresForDisplay = moveableSquaresWhenPinned(piecePosition) & squaresToBlockCheckOrCapture;
		else
			movableSquaresForDisplay = HorzNVerticalMoves(piecePosition) & notCapturable & squaresToBlockCheckOrCapture;
	}

	unsigned long long moveableSquaresWhenPinned(const int& piecePosition) {
		if (isWhite) 	{
			return moveSqPinnedHorzVert(piecePosition, whKing);
		}
		else
			return moveSqPinnedHorzVert(piecePosition, blKing);

		std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
	}

	void updateAttackSquares(unsigned long long pieceBitBoard, unsigned long long kingBitBoard, const unsigned long long& enemyKingLociSpread, const unsigned long long& myPieces) {
		attackSquaresRook = 0ULL;
		updateAttackSqHrzVertBB(pieceBitBoard, kingBitBoard, enemyKingLociSpread, myPieces, attackSquaresRook);
	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite)
			return legalMoves(whRook);
		else
			return legalMoves(blRook);
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		possibleMoves->reserve(12);
		uint16_t mergeOfBeforeNAfterMove;
		unsigned long long allPotentialMoves = 0ULL;

		while (pieceBitBoard != 0) {
			int rookLocation = numOfTrailingZeros(pieceBitBoard);
			
			if (((1ULL << rookLocation) & pinnedPiecesBitBoard) != 0)//if piece is pinned
				allPotentialMoves = moveableSquaresWhenPinned(rookLocation) & squaresToBlockCheckOrCapture;
			else
				allPotentialMoves = HorzNVerticalMoves(rookLocation) & notCapturable & squaresToBlockCheckOrCapture;

			unsigned long long aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);

			while (aPotentialMove != 0) {
				int movedSquare = numOfTrailingZeros(aPotentialMove);
				mergeOfBeforeNAfterMove = rookLocation;
				mergeOfBeforeNAfterMove |= (movedSquare << 8);
				possibleMoves->push_back(mergeOfBeforeNAfterMove);

				allPotentialMoves &= ~(aPotentialMove);
				aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			}
			pieceBitBoard &= ~(1ULL << rookLocation);
		}
		
		return possibleMoves;
	}
};

#endif