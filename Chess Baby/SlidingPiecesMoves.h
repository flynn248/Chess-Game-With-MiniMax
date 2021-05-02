#include "BoardInfo.h"
#pragma once

struct SlidingPiecesMoves : public BoardInfo				{
	unsigned long long whMoveSquaresPinnedHrzVer(const int& piecePosition) {
		if ((rankMasks[piecePosition / 8] & whKing) != 0) //if king is on same rank (horz)
			return horizontalMoves(piecePosition) & notCapturable;
		else if ((fileMasks[piecePosition % 8] & whKing) != 0) //if king is on same file (vert)
			return verticalMoves(piecePosition) & notCapturable;
		else
			return 0ULL;

	}

	unsigned long long blMoveSquaresPinnedHrzVer(const int& piecePosition, const unsigned long long& myKing) {
		if ((rankMasks[piecePosition / 8] & blKing) != 0)  //if king is on same rank (horz)
			return horizontalMoves(piecePosition) & notCapturable;
		else if ((fileMasks[piecePosition % 8] & blKing) != 0) //if king is on same file (vert)
			return verticalMoves(piecePosition) & notCapturable;
		else
			return 0ULL;
	}

	unsigned long long moveSqPinnedHorzVert(const int& piecePosition, const unsigned long long& myKing) {
		if ((rankMasks[piecePosition / 8] & myKing) != 0)  //if king is on same rank (horz)
			return horizontalMoves(piecePosition) & notCapturable;
		else if ((fileMasks[piecePosition % 8] & myKing) != 0) //if king is on same file (vert)
			return verticalMoves(piecePosition) & notCapturable;
		else
			return 0ULL;
	}

	unsigned long long moveSqPinnedDiagAntiD(const int& piecePosition, const unsigned long long& myKing) {
		if ((rankMasks[piecePosition / 8] & myKing) != 0)  //if king is on same rank (horz)
			return horizontalMoves(piecePosition) & notCapturable;
		else if ((fileMasks[piecePosition % 8] & myKing) != 0) //if king is on same file (vert)
			return verticalMoves(piecePosition) & notCapturable;
		else
			return 0ULL;
	}
};