#ifndef QUEEN_H
#define QUEEN_H
#include "Piece.h"
#include <string>

class Queen : public Piece {


public:
	Queen() {

	}
	Queen(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition) {
	}
	void draw(sf::RenderWindow& window) {
		Piece::drawPiece(window);
	}

	void findMoveableSquares(const int& piecePosition) {
		singlePieceMoveableSquares(piecePosition);
	}
	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if (((1ULL << piecePosition) & pinnedPiecesBitBoard) != 0) //if piece is pinned
			movableSquaresForDisplay = moveableSquaresWhenPinned(piecePosition);
		else //if not
			movableSquaresForDisplay = (HorzNVerticalMoves(piecePosition) | diagNAntiDagMoves(piecePosition)) & notCapturable & squaresToBlockCheckOrCapture;
	}

	void updateAttackSquares(unsigned long long pieceBitBoard, unsigned long long kingBitBoard, unsigned long long& enemyKingLociSpread, unsigned long long& myPieces) {
		unsigned long long queenPiece = pieceBitBoard;
		unsigned long long aPathToAttackKing = 0ULL;
		unsigned long long oneNegatedPiece = 0ULL;
		unsigned long long uneditedAttackPath = 0ULL;
		attackSquaresQueen = 0ULL;

		while (queenPiece != 0) {
			int queenLocation = numOfTrailingZeros(queenPiece);
			oneNegatedPiece = (1ULL << queenLocation) * -1;

			//for finding a pinned piece
			uneditedAttackPath = diagonalMoves(queenLocation);
			if ((diagonalMasks[(queenLocation / 8) + (queenLocation % 8)] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << queenLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);

				checkPathXRayThroughKing |= diagonalMasks[(queenLocation / 8) + (queenLocation % 8)];
			}

			attackSquaresQueen |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = antiDiagMoves(queenLocation);
			//for finding a pinned piece
			if ((antiDiagonalMasks[(queenLocation / 8) + 7 - (queenLocation % 8)] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << queenLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);

				checkPathXRayThroughKing |= antiDiagonalMasks[(queenLocation / 8) + 7 - (queenLocation % 8)];
			}

			attackSquaresQueen |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = verticalMoves(queenLocation);
			//for finding a pinned piece
			if ((fileMasks[queenLocation % 8] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << queenLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);

				checkPathXRayThroughKing |= fileMasks[queenLocation % 8];
			}

			attackSquaresQueen |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = horizontalMoves(queenLocation);
			//for finding a pinned piece
			if ((rankMasks[queenLocation / 8] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << queenLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << queenLocation);

				checkPathXRayThroughKing |= rankMasks[queenLocation / 8];
			}

			attackSquaresQueen |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			pieceBitBoard &= ~(1ULL << queenLocation);
			queenPiece = pieceBitBoard;
		}

	}

	unsigned long long moveableSquaresWhenPinned(const int& piecePosition) {
		if (isWhite) {
			if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & whKing) != 0) //if king is at a diagonal
				return diagonalMoves(piecePosition) & notCapturable;
			else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & whKing) != 0) //if king is at antiDiagonal
				return antiDiagMoves(piecePosition) & notCapturable;
			else if ((rankMasks[piecePosition / 8] & whKing) != 0) //if king is on same rank (horz)
				return horizontalMoves(piecePosition) & notCapturable;
			else if ((fileMasks[piecePosition % 8] & whKing) != 0) //if king is on same file (vert)
				return verticalMoves(piecePosition) & notCapturable;
			else
				std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
		}
		else {
			if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & blKing) != 0) //if king is at a diagonal
				return diagonalMoves(piecePosition) & notCapturable;
			else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & blKing) != 0) //if king is at antiDiagonal
				return antiDiagMoves(piecePosition) & notCapturable;
			else if ((rankMasks[piecePosition / 8] & blKing) != 0) //if king is on same rank (horz)
				return horizontalMoves(piecePosition) & notCapturable;
			else if ((fileMasks[piecePosition % 8] & blKing) != 0) //if king is on same file (vert)
				return verticalMoves(piecePosition) & notCapturable;
			else
				std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
		}
	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite) {
			return legalMoves(whQueen);
		}
		else {
			return legalMoves(blQueen);
		}
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) { //Queen legal moves
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;
		unsigned long long queenPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;

		while (queenPiece != 0) {
			int queenLocation = numOfTrailingZeros(queenPiece);

			if (((1ULL << queenLocation) & pinnedPiecesBitBoard) != 0) //if piece is pinned
				movableSquaresForDisplay = moveableSquaresWhenPinned(queenLocation);
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
			queenPiece = pieceBitBoard;
		}
		
		return possibleMoves;
	}
};

#endif