#ifndef QUEEN_H
#define QUEEN_H
#include "Piece.h"
#include "SlidingPiecesMoves.h"
#include <string>

class Queen : public Piece, private SlidingPiecesMoves {

	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if (((1ULL << piecePosition) & pinnedPiecesBitBoard) != 0) //if piece is pinned
			movableSquaresForDisplay = moveableSquaresWhenPinned(piecePosition) & squaresToBlockCheckOrCapture;
		else //if not
			movableSquaresForDisplay = (HorzNVerticalMoves(piecePosition) | diagNAntiDagMoves(piecePosition)) & notCapturable & squaresToBlockCheckOrCapture;
	}

	unsigned long long moveableSquaresWhenPinned(const int& piecePosition) {
		if (isWhite) {
			return moveSqPinnedHzVtDiAnD(piecePosition, whKing);
		}
		return moveSqPinnedHzVtDiAnD(piecePosition, blKing);

		//std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
	}
public:
	Queen() {}
	Queen(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition, const int numPieces)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition, numPieces) {
	}
	void draw(sf::RenderWindow& window) { Piece::drawPiece(window); }

	void findMoveableSquares(const int& piecePosition) { singlePieceMoveableSquares(piecePosition); }

	void updateAttackSquares(unsigned long long pieceBitBoard, unsigned long long kingBitBoard, unsigned long long& enemyKingLociSpread, unsigned long long& myPieces) {
		attackSquaresQueen = 0ULL;
		//updateAttackSqHrzVert(pieceLocation, numPieces, kingBitBoard, enemyKingLociSpread, myPieces, attackSquaresQueen);
		//updateAttackSqDiagAntiDi(pieceLocation, numPieces, kingBitBoard, enemyKingLociSpread, myPieces, attackSquaresQueen);
		updateAttackSqHrzVertBB(pieceBitBoard, kingBitBoard, enemyKingLociSpread, myPieces, attackSquaresQueen);
		updateAttackSqDiagAntiDiBB(pieceBitBoard, kingBitBoard, enemyKingLociSpread, myPieces, attackSquaresQueen);

		
	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite) 
			return legalMoves(whQueen);
		else
			return legalMoves(blQueen);
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) { //Queen legal moves
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		possibleMoves->reserve(12);

		uint16_t mergeOfBeforeNAfterMove;
		unsigned long long allPotentialMoves = 0ULL;
		
		while (pieceBitBoard != 0) {
			int queenLocation = numOfTrailingZeros(pieceBitBoard);

			if (((1ULL << queenLocation) & pinnedPiecesBitBoard) != 0) //if piece is pinned
				allPotentialMoves = moveableSquaresWhenPinned(queenLocation) & squaresToBlockCheckOrCapture;
			else //if not
				allPotentialMoves = (HorzNVerticalMoves(queenLocation) | diagNAntiDagMoves(queenLocation)) & notCapturable & squaresToBlockCheckOrCapture;
			
			unsigned long long aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);

			while (aPotentialMove != 0) {
				int movedSquare = numOfTrailingZeros(aPotentialMove);
				mergeOfBeforeNAfterMove = queenLocation;
				mergeOfBeforeNAfterMove |= (movedSquare << 8);
				possibleMoves->push_back(mergeOfBeforeNAfterMove);

				allPotentialMoves &= ~(aPotentialMove);
				aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			}
			pieceBitBoard &= ~(1ULL << queenLocation);
		}
		
		return possibleMoves;
	}
};

#endif