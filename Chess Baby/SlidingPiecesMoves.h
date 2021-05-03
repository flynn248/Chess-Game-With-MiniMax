//Class to hold move generation for sliding pieces
#include "BoardInfo.h"
#pragma once

struct SlidingPiecesMoves : public BoardInfo				{
	static unsigned long long moveSqPinnedHorzVert(const int& piecePosition, const unsigned long long& myKing) {
		if ((rankMasks[piecePosition / 8] & myKing) != 0)  //if king is on same rank (horz)
			return horizontalMoves(piecePosition) & notCapturable;
		else if ((fileMasks[piecePosition % 8] & myKing) != 0) //if king is on same file (vert)
			return verticalMoves(piecePosition) & notCapturable;
		else
			return 0ULL;
	}
	static unsigned long long moveSqPinnedDiagAntiD(const int& piecePosition, const unsigned long long& myKing) {
		if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & myKing) != 0) //if king is at a diagonal
			return diagonalMoves(piecePosition) & notCapturable;
		else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & myKing) != 0) //if king is at antiDiagonal
			return antiDiagMoves(piecePosition) & notCapturable;
		else
			return 0ULL;
	}
	static unsigned long long moveSqPinnedHzVtDiAnD(const int& piecePosition, const unsigned long long& myKing) {
		if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & myKing) != 0) //if king is at a diagonal
			return diagonalMoves(piecePosition) & notCapturable;
		else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & myKing) != 0) //if king is at antiDiagonal
			return antiDiagMoves(piecePosition) & notCapturable;
		else if ((rankMasks[piecePosition / 8] & myKing) != 0) //if king is on same rank (horz)
			return horizontalMoves(piecePosition) & notCapturable;
		else if ((fileMasks[piecePosition % 8] & myKing) != 0) //if king is on same file (vert)
			return verticalMoves(piecePosition) & notCapturable;
		else
			return 0ULL;
	}

	static void updateAttackSqHrzVertBB(unsigned long long pieceBitBoard, const unsigned long long& kingBitBoard,
		const unsigned long long& enemyKingLociSpread, const unsigned long long& myPieces, unsigned long long& attackSquaresPiece) {
		unsigned long long aPieceBB = pieceBitBoard;
		unsigned long long aPathToAttackKing = 0ULL;
		unsigned long long oneNegatedPiece = 0ULL;
		unsigned long long uneditedAttackPath = 0ULL;

		while (aPieceBB != 0) {
			int aPieceLoc = numOfTrailingZeros(aPieceBB);
			oneNegatedPiece = (1ULL << aPieceLoc) * -1;
			uneditedAttackPath = verticalMoves(aPieceLoc);

			//for finding a pinned piece
			if ((fileMasks[aPieceLoc % 8] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);

				checkPathXRayThroughKing |= fileMasks[aPieceLoc % 8];
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
			}

			attackSquaresPiece |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = horizontalMoves(aPieceLoc);
			//for finding a pinned piece
			if ((rankMasks[aPieceLoc / 8] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);

				checkPathXRayThroughKing |= rankMasks[aPieceLoc / 8];
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
			}

			attackSquaresPiece |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			pieceBitBoard &= ~(1ULL << aPieceLoc); //remove bishop from bitmap of bishops
			aPieceBB = pieceBitBoard; //grab next bishop on map
		}
	}
	static void updateAttackSqDiagAntiDiBB(unsigned long long pieceBitBoard, const unsigned long long& kingBitBoard,
		const unsigned long long& enemyKingLociSpread, const unsigned long long& myPieces, unsigned long long& attackSquaresPiece) {
		
		unsigned long long aPieceBB = pieceBitBoard;
		unsigned long long aPathToAttackKing = 0ULL;
		unsigned long long oneNegatedPiece = 0ULL;
		unsigned long long uneditedAttackPath = 0ULL;

		while (aPieceBB != 0) {
			int aPieceLoc = numOfTrailingZeros(aPieceBB);
			oneNegatedPiece = (1ULL << aPieceLoc) * -1;

			//for finding a pinned piece
			uneditedAttackPath = diagonalMoves(aPieceLoc);
			if ((diagonalMasks[(aPieceLoc / 8) + (aPieceLoc % 8)] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) { //for check purposes
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);

				checkPathXRayThroughKing |= diagonalMasks[(aPieceLoc / 8) + (aPieceLoc % 8)];
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
			}

			attackSquaresPiece |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = antiDiagMoves(aPieceLoc);
			//for finding a pinned piece
			if ((antiDiagonalMasks[(aPieceLoc / 8) + 7 - (aPieceLoc % 8)] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable; //Used to have & (notCapturable | kingBitBoard)

			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << aPieceLoc);

				checkPathXRayThroughKing |= antiDiagonalMasks[(aPieceLoc / 8) + 7 - (aPieceLoc % 8)];
				locationOfPieceAttackingKing |= 1ULL << aPieceLoc;
			}

			attackSquaresPiece |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			pieceBitBoard &= ~(1ULL << aPieceLoc); //remove bishop from bitmap of bishops
			aPieceBB = pieceBitBoard; //grab next bishop on map
		}
	}
};