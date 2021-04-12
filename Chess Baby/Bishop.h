#ifndef BISHOP_H
#define BISHOP_H
#include "Piece.h"
#include <string>

class Bishop : public Piece {


public:
	Bishop() {

	}
	Bishop(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition)
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
		if (((1ULL << piecePosition) & pinnedPiecesBitBoard) != 0)//if piece is pinned
			movableSquaresForDisplay = moveableSquaresWhenPinned(piecePosition);
		else //if piece is not pinned
			movableSquaresForDisplay = diagNAntiDagMoves(piecePosition) & notCapturable & squaresToBlockCheckOrCapture;
	}

	void updateAttackSquares(unsigned long long pieceBitBoard, unsigned long long kingBitBoard, unsigned long long& enemyKingLociSpread, unsigned long long& myPieces) { //, unsigned long long& piecesThatArePinned
		unsigned long long bishopPiece = pieceBitBoard;
		unsigned long long aPathToAttackKing = 0ULL;
		unsigned long long oneNegatedPiece = 0ULL;
		unsigned long long uneditedAttackPath = 0ULL;
		attackSquaresBishop = 0ULL;

		while (bishopPiece != 0) {
			int bishopLocation = numOfTrailingZeros(bishopPiece);
			oneNegatedPiece = (1ULL << bishopLocation) * -1;

			//for finding a pinned piece
			uneditedAttackPath = diagonalMoves(bishopLocation);
			if ((diagonalMasks[(bishopLocation / 8) + (bishopLocation % 8)] & kingBitBoard) != 0) 	{ //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) 	{ //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
					std::cout << "You a bitch\n";
				}
			}
						
			aPathToAttackKing = uneditedAttackPath & notCapturable;
			if ((aPathToAttackKing & kingBitBoard) != 0) 	{ //for check purposes
				locationOfPieceAttackingKing |= 1ULL << bishopLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << bishopLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << bishopLocation);

				checkPathXRayThroughKing |= diagonalMasks[(bishopLocation / 8) + (bishopLocation % 8)];
			}

			attackSquaresBishop |= aPathToAttackKing;			
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			uneditedAttackPath = antiDiagMoves(bishopLocation); 
			//for finding a pinned piece
			if ((antiDiagonalMasks[(bishopLocation / 8) + 7 - (bishopLocation % 8)] & kingBitBoard) != 0) { //if enemy king is in attack path
				if (((uneditedAttackPath & ~myPieces) & enemyKingLociSpread) != 0) { //if one enemy piece is on path of attack to king
					pinnedPiecesBitBoard |= (uneditedAttackPath & ~myPieces) & enemyKingLociSpread;
					std::cout << "You a bitch\n";
				}
			}

			aPathToAttackKing = uneditedAttackPath & notCapturable; //Used to have & (notCapturable | kingBitBoard)

			if ((aPathToAttackKing & kingBitBoard) != 0) {
				locationOfPieceAttackingKing |= 1ULL << bishopLocation;
				if (((oneNegatedPiece & aPathToAttackKing) & kingBitBoard) != 0)
					squaresToBlockCheckOrCapture |= oneNegatedPiece & aPathToAttackKing | (1ULL << bishopLocation);
				else
					squaresToBlockCheckOrCapture |= ~oneNegatedPiece & aPathToAttackKing | (1ULL << bishopLocation);
				
				checkPathXRayThroughKing |= antiDiagonalMasks[(bishopLocation / 8) + 7 - (bishopLocation % 8)];
			}

			attackSquaresBishop |= aPathToAttackKing;
			enemyPiecesThatAreDefended |= uneditedAttackPath & myPieces;

			pieceBitBoard &= ~(1ULL << bishopLocation); //remove bishop from bitmap of bishops
			bishopPiece = pieceBitBoard; //grab next bishop on map
		}
	}
	
	unsigned long long moveableSquaresWhenPinned(const int& piecePosition) {
		if (isWhite) {
			if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & whKing) != 0) //if king is at a diagonal
				return diagonalMoves(piecePosition) & notCapturable;
			else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & whKing) != 0) //if king is at antiDiagonal
				return antiDiagMoves(piecePosition) & notCapturable;
			else
				std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
		}
		else {
			if ((diagonalMasks[(piecePosition / 8) + (piecePosition % 8)] & blKing) != 0) //if king is at a diagonal
				return diagonalMoves(piecePosition) & notCapturable;
			else if ((antiDiagonalMasks[(piecePosition / 8) + 7 - (piecePosition % 8)] & blKing) != 0) //if king is at antiDiagonal
				return antiDiagMoves(piecePosition) & notCapturable;
			else
				std::cout << "ERROR: Failed to find path from pinned piece to king!\n";
		}
	}
	/*
	void findIfPiecesIsPinned(unsigned long long& cake) {

	}
	void findPathToCheck(unsigned long long attackerBitBoard, unsigned long long kingBitBoard) { //unused code
		unsigned long long bishopPiece = attackerBitBoard;
		unsigned long long aPathToAttackKing = 0ULL;

		while (bishopPiece != 0) 	{
			int bishopLocation = numOfTrailingZeros(bishopPiece);
			aPathToAttackKing = diagonalMoves(bishopLocation) & (notCapturable | kingBitBoard);
			if ((aPathToAttackKing & kingBitBoard) != 0)
				squaresToBlockCheckOrCapture |= (aPathToAttackKing ^ kingBitBoard);

			aPathToAttackKing = antiDiagMoves(bishopLocation) & (notCapturable | kingBitBoard);
			if ((aPathToAttackKing & kingBitBoard) != 0)
				squaresToBlockCheckOrCapture |= (aPathToAttackKing ^ kingBitBoard);

			attackerBitBoard &= ~(1ULL << bishopLocation);
			bishopPiece = attackerBitBoard;
		}
	}
	*/

	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { //Get legal moves for human player
		if (isWhite) {
			return legalMoves(whBishop);
		}
		else {
			return legalMoves(blBishop);
		}
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) { //might want to add parameters to have something more local for whBishop. Might cause update issues later
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;
		unsigned long long bishopPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;

		//attackSquaresBishop = 0ULL;

		while (bishopPiece != 0) {
			int bishopLocation = numOfTrailingZeros(bishopPiece);
			allPotentialMoves = diagNAntiDagMoves(bishopLocation) & notCapturable & squaresToBlockCheckOrCapture;
			//attackSquaresBishop |= allPotentialMoves;
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

//void updateAttackSquares(unsigned long long pieceBitBoard) {
//	unsigned long long bishopPiece = pieceBitBoard;
//	unsigned long long allPotentialMoves = 0ULL;
//
//	attackSquaresBishop = 0ULL;
//
//	while (bishopPiece != 0) {
//		int bishopLocation = numOfTrailingZeros(bishopPiece);
//		allPotentialMoves = diagNAntiDagMoves(bishopLocation) & notCapturable;
//		attackSquaresBishop |= allPotentialMoves;
//
//		pieceBitBoard &= ~(1ULL << bishopLocation); //remove bishop from bitmap of bishops
//		bishopPiece = pieceBitBoard; //grab next bishop on map
//	}
//
//}