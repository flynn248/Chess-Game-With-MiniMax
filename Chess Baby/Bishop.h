#ifndef BISHOP_H
#define BISHOP_H
#include "Piece.h"
#include "SlidingPiecesMoves.h"
#include <string>

class Bishop : public Piece, private SlidingPiecesMoves {

public:
	Bishop(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition, const int numPieces)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition, numPieces) {
	}
	void drawPiece(sf::RenderWindow& window) { Piece::drawPiece(window); }

	void findMoveableSquares(const int& piecePosition) { singlePieceMoveableSquares(piecePosition); }
	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if (((1ULL << piecePosition) & pinnedPiecesBitBoard) != 0)//if piece is pinned
			movableSquaresForDisplay = moveableSquaresWhenPinned(piecePosition) & squaresToBlockCheckOrCapture;
		else //if piece is not pinned
			movableSquaresForDisplay = diagNAntiDagMoves(piecePosition) & notCapturable & squaresToBlockCheckOrCapture;
	}

	void updateAttackSquares(unsigned long long pieceBitBoard, unsigned long long kingBitBoard, unsigned long long& enemyKingLociSpread, unsigned long long& myPieces) { //, unsigned long long& piecesThatArePinned
		attackSquaresBishop = 0ULL;
		updateAttackSqDiagAntiDiBB(pieceBitBoard, kingBitBoard, enemyKingLociSpread, myPieces, attackSquaresBishop);		
	}
	
	unsigned long long moveableSquaresWhenPinned(const int& piecePosition) {
		if (isWhite) {
			return moveSqPinnedDiagAntiD(piecePosition, whKing);
		}
		else
			return moveSqPinnedDiagAntiD(piecePosition, blKing);

		std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite) 
			return legalMoves(whBishop);
		else 
			return legalMoves(blBishop);
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) { //might want to add parameters to have something more local for whBishop. Might cause update issues later
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;
		unsigned long long bishopPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;
		
		while (bishopPiece != 0) {
			int bishopLocation = numOfTrailingZeros(bishopPiece);
			if (((1ULL << bishopLocation) & pinnedPiecesBitBoard) != 0)//if piece is pinned
				allPotentialMoves = moveableSquaresWhenPinned(bishopLocation) & squaresToBlockCheckOrCapture;
			else
				allPotentialMoves = diagNAntiDagMoves(bishopLocation) & notCapturable & squaresToBlockCheckOrCapture;
			
			unsigned long long aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);

			while (aPotentialMove != 0) {
				int movedSquare = numOfTrailingZeros(aPotentialMove);
				mergeOfBeforeNAfterMove = bishopLocation;
				mergeOfBeforeNAfterMove |= (movedSquare << 8);
				possibleMoves->push_back(mergeOfBeforeNAfterMove);

				allPotentialMoves &= ~(aPotentialMove);
				aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			}
			pieceBitBoard &= ~(1ULL << bishopLocation); //remove bishop from bitmap of bishops
			bishopPiece = pieceBitBoard; //grab next bishop on map
		}
		
		return possibleMoves;
	}
};

#endif