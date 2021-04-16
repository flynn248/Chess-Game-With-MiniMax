#ifndef ROOK_H
#define ROOK_H
#include "Piece.h"
#include <string>

class Rook : public Piece {
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
	
	void findMoveableSquares(const int& piecePosition) {
			singlePieceMoveableSquares(piecePosition);
	}
	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if (((1ULL << piecePosition) & pinnedPiecesBitBoard) != 0)//if piece is pinned
			movableSquaresForDisplay = moveableSquaresWhenPinned(piecePosition);
		else
			movableSquaresForDisplay = HorzNVerticalMoves(piecePosition) & notCapturable & squaresToBlockCheckOrCapture;
	}

	unsigned long long moveableSquaresWhenPinned(const int& piecePosition) {
		if (isWhite) {
			if ((rankMasks[piecePosition / 8] & whKing) != 0) //if king is on same rank (horz)
				return horizontalMoves(piecePosition) & notCapturable;
			else if ((fileMasks[piecePosition % 8] & whKing) != 0) //if king is on same file (vert)
				return verticalMoves(piecePosition) & notCapturable;
			else
				return 0ULL;
		}
		else {
			if ((rankMasks[piecePosition / 8] & blKing) != 0)  //if king is on same rank (horz)
				return horizontalMoves(piecePosition) & notCapturable;
			else if ((fileMasks[piecePosition % 8] & blKing) != 0) //if king is on same file (vert)
				return verticalMoves(piecePosition) & notCapturable;
			else
				return 0ULL;
		}
		std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
	}

	void updateAttackSquares(unsigned long long pieceBitBoard, unsigned long long kingBitBoard, unsigned long long& enemyKingLociSpread, unsigned long long& myPieces) {
		unsigned long long rookPiece = pieceBitBoard;
		unsigned long long aPathToAttackKing = 0ULL;
		unsigned long long oneNegatedPiece = 0ULL;
		unsigned long long uneditedAttackPath = 0ULL;
		attackSquaresRook = 0ULL;

		while (rookPiece != 0) {
			int rookLocation = numOfTrailingZeros(rookPiece);
			oneNegatedPiece = (1ULL << rookLocation) * -1;

			uneditedAttackPath = verticalMoves(rookLocation);
			//for finding a pinned piece
			if ((fileMasks[rookLocation % 8] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << rookLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << rookLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << rookLocation);

				checkPathXRayThroughKing |= fileMasks[rookLocation % 8];
				locationOfPieceAttackingKing |= 1ULL << rookLocation;
			}

			attackSquaresRook |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = horizontalMoves(rookLocation);
			//for finding a pinned piece
			if ((rankMasks[rookLocation / 8] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << rookLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << rookLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << rookLocation);

				checkPathXRayThroughKing |= rankMasks[rookLocation / 8];
				locationOfPieceAttackingKing |= 1ULL << rookLocation;
			}

			attackSquaresRook |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			pieceBitBoard &= ~(1ULL << rookLocation);
			rookPiece = pieceBitBoard;
		}

	}

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite)
			return legalMoves(whRook);
		else
			return legalMoves(blRook);
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;
		unsigned long long rookPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;

		while (rookPiece != 0) {
			int rookLocation = numOfTrailingZeros(rookPiece);
			
			if (((1ULL << rookLocation) & pinnedPiecesBitBoard) != 0)//if piece is pinned
				allPotentialMoves = moveableSquaresWhenPinned(rookLocation);
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
			rookPiece = pieceBitBoard;
		}
		
		return possibleMoves;
	}
};

#endif