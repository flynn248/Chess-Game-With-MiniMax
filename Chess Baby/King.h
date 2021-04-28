#ifndef KING_H
#define KING_H
#include "Piece.h"
#include "Rook.h"
#include <string>

class King : public Piece {
private:
	unsigned long long squaresTheEnemyAttacks = 0ULL;
	//bool hasLeftRookMoved,
	//	hasRightRookMoved;
	bool hasKingMoved;
	unsigned long long spotsToRightRook = 0ULL;
	unsigned long long spotsToLeftRook = 0ULL;
public:
	King() {

	}
	King(const int& val, const float& scale, const std::string& fileName, const float& posX, const bool& isWhite, const std::string name, unsigned long long& bitBoardPosition, const int numPieces)
		: Piece(val, scale, fileName, posX, isWhite, name, bitBoardPosition, numPieces) {
	}
	void drawPiece(sf::RenderWindow& window) { // dead code. Not needed as of yet
		Piece::drawPiece(window);
	}
	void undoHasKingMoved() { hasKingMoved = false; }
	void findMoveableSquares(const int& piecePosition) {
		singlePieceMoveableSquares(piecePosition);
		
		if (numberOfChecks != 0)
			return;
		
		if (isWhite)
			singlePieceCastleSquaresWhite();
		else
			singlePieceCastleSquaresBlack();
	}
	void singlePieceMoveableSquares(const int& piecePosition) {
		movableSquaresForDisplay = 0ULL;
		if (piecePosition > 9)
			movableSquaresForDisplay |= KingSpan << (piecePosition - 9);
		else
			movableSquaresForDisplay |= KingSpan >> (9 - piecePosition);

		unsigned long long pieceAttackingKing = 0ULL;
		if ((enemyPiecesThatAreDefended & locationOfPieceAttackingKing) == 0) //if piece attacking king is not defended
			pieceAttackingKing = locationOfPieceAttackingKing;
		
		if (piecePosition % 8 < 4)  //prevent magical looping of king to other side of board
			movableSquaresForDisplay &= ~FILE_GH & notCapturable & ~squaresTheEnemyAttacks & ~checkPathXRayThroughKing & ~enemyPiecesThatAreDefended | pieceAttackingKing;
		else
			movableSquaresForDisplay &= ~FILE_AB & notCapturable & ~squaresTheEnemyAttacks & ~checkPathXRayThroughKing & ~enemyPiecesThatAreDefended | pieceAttackingKing;

	}
	void singlePieceCastleSquaresWhite() {
		//Includes king and rook in spots for extra safeguard to prevent castling while in check
		spotsToLeftRook = 0x1F00000000000000;
		spotsToRightRook = 0xF000000000000000;

		if (hasKingMoved || castleRooks[0] == -1 && castleRooks[1] == -1 ) //if both rooks or king moved
			return;

		if (castleRooks[0] != -1) //if right rook has not moved or been captured yet
			if ((spotsToRightRook & ~squaresBlackAttacks & (notCapturable ^ 0x9000000000000000)) == spotsToRightRook) //if no pieces attack castling squares
				movableSquaresForDisplay ^= 0x4000000000000000;

		if (castleRooks[1] != -1) //if left rook has not moved or been captured yet
			if ((spotsToLeftRook & ~squaresBlackAttacks & (notCapturable ^ 0x1100000000000000)) == spotsToLeftRook) //if no pieces attack castling squares
				movableSquaresForDisplay ^= 0x400000000000000;
	}
	void singlePieceCastleSquaresBlack() {
		if (hasKingMoved || castleRooks[2] == -1 && castleRooks[3] == -1) //if both rooks or king moved
			return;

		//Includes king and rook in spots for extra safeguard to prevent castling while in check
		spotsToLeftRook = 0x1F;
		spotsToRightRook = 0xF0;

		if (castleRooks[2] != -1) //if right rook, from white's perspective, has not moved or been captured yet
			if ((spotsToRightRook & ~squaresWhiteAttacks & (notCapturable ^ 144)) == spotsToRightRook)  //if no pieces attack castling squares
				movableSquaresForDisplay ^= 64;

		if (castleRooks[3] != -1)  //if left rook has not moved or been captured yet
			if ((spotsToLeftRook & ~squaresWhiteAttacks & (notCapturable ^ 17)) == spotsToLeftRook)  //if no pieces attack castling squares
				movableSquaresForDisplay ^= 4;
	}
	void setSquaresTheEnemyAttacks(unsigned long long enemy) {
		squaresTheEnemyAttacks = enemy;
	}
	void updateHasKingMoved() {
		hasKingMoved = true;
	}
	void updateAttackSquares(unsigned long long& pieceBitBoard, const unsigned long long& myPieces) {
		unsigned long long allPotentialMoves = 0ULL;

		int kingLocation = numOfTrailingZeros(pieceBitBoard);
		if (kingLocation > 9)
			allPotentialMoves = KingSpan << (kingLocation - 9);
		else
			allPotentialMoves = KingSpan >> (9 - kingLocation);

		enemyPiecesThatAreDefended |= allPotentialMoves & myPieces;
		
		if (kingLocation % 8 < 4)  //prevent magical looping of king to other side of board
			allPotentialMoves &= ~FILE_GH & notCapturable;
		else
			allPotentialMoves &= ~FILE_AB & notCapturable;

		attackSquaresKing = 0ULL;
		attackSquaresKing |= allPotentialMoves;
	}

	void setHasKingMoved(bool& hkm) { hasKingMoved = hkm; }
	bool getHasKingMoved() const { return hasKingMoved; }
	unsigned long long getSquaresTheEnemyAttacks() const { return squaresTheEnemyAttacks; }


	std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() {
		std::unique_ptr<std::vector<uint16_t>> allPossibleMoves = std::make_unique<std::vector<uint16_t>>(),
			standardMoves = std::make_unique<std::vector<uint16_t>>(),
			castlingMoves = std::make_unique<std::vector<uint16_t>>();

		if (isWhite) {
			standardMoves = legalMoves(whKing);
			castlingMoves = castlingMovesWhite();
		}
		else {
			standardMoves = legalMoves(blKing);
			castlingMoves = castlingMovesBlack();
		}

		allPossibleMoves->insert(allPossibleMoves->end(), standardMoves->begin(), standardMoves->end());
		allPossibleMoves->insert(allPossibleMoves->end(), castlingMoves->begin(), castlingMoves->end());

		return allPossibleMoves;
	}

	std::unique_ptr<std::vector<uint16_t>> legalMoves(unsigned long long pieceBitBoard) {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		uint16_t mergeOfBeforeNAfterMove;

		unsigned long long kingPiece = pieceBitBoard;
		unsigned long long allPotentialMoves = 0ULL;
		
		while (kingPiece != 0) { //find center of king
			int kingLocation = numOfTrailingZeros(kingPiece);
			if (kingLocation > 9)
				allPotentialMoves = KingSpan << (kingLocation - 9);
			else
				allPotentialMoves = KingSpan >> (9 - kingLocation);

			unsigned long long pieceAttackingKing = 0ULL;
			if ((enemyPiecesThatAreDefended & locationOfPieceAttackingKing) == 0) //if piece attacking king is not defended
				pieceAttackingKing = locationOfPieceAttackingKing;

			if (kingLocation % 8 < 4)  //prevent magical looping of king to other side of board
				allPotentialMoves &= ~FILE_GH & notCapturable & ~squaresTheEnemyAttacks & ~checkPathXRayThroughKing & ~enemyPiecesThatAreDefended | pieceAttackingKing;
			else
				allPotentialMoves &= ~FILE_AB & notCapturable & ~squaresTheEnemyAttacks & ~checkPathXRayThroughKing & ~enemyPiecesThatAreDefended | pieceAttackingKing;

			unsigned long long aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			while (aPotentialMove != 0) {
				int movedSquare = numOfTrailingZeros(aPotentialMove);
				mergeOfBeforeNAfterMove = kingLocation;
				mergeOfBeforeNAfterMove |= (movedSquare << 8);
				possibleMoves->push_back(mergeOfBeforeNAfterMove);

				allPotentialMoves &= ~(aPotentialMove);
				aPotentialMove = allPotentialMoves & ~(allPotentialMoves - 1);
			}
			pieceBitBoard &= ~(1ULL << kingLocation);
			kingPiece = pieceBitBoard & ~(pieceBitBoard - 1);
		}
		
		return possibleMoves;
	}

	std::unique_ptr<std::vector<uint16_t>> castlingMovesWhite() {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();

		if (hasKingMoved || castleRooks[0] == -1 && castleRooks[1] == -1) 	{ //if both rooks or king moved
			return possibleMoves;
		}

		uint16_t rookBeforeAndAfter; //the king moves a constant amount and can be found later
		
		//Includes king and rook in spots for extra safeguard to prevent castling while in check
		spotsToLeftRook = 0x1F00000000000000;
		spotsToRightRook = 0xF000000000000000;

		if (castleRooks[0] != -1) 	{ //if right rook has not moved or been captured yet
			if ((spotsToRightRook & ~squaresBlackAttacks & (notCapturable ^ 0x9000000000000000)) == spotsToRightRook) { //if no pieces attack castling squares
				rookBeforeAndAfter = 63;
				rookBeforeAndAfter |= (189 << 8); //189 = 10111101. First One signifies it was a castling move
				possibleMoves->push_back(rookBeforeAndAfter);
			}
		}

		if (castleRooks[1] != -1) { //if left rook has not moved or been captured yet
			if ((spotsToLeftRook & ~squaresBlackAttacks & (notCapturable ^ 0x1100000000000000)) == spotsToLeftRook) { //if no pieces attack castling squares
				rookBeforeAndAfter = 56;
				rookBeforeAndAfter |= (187 << 8);
				possibleMoves->push_back(rookBeforeAndAfter);
			}
		}

		return possibleMoves;
	}
	std::unique_ptr<std::vector<uint16_t>> castlingMovesBlack() {
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();

		if (hasKingMoved || castleRooks[2] == -1 && castleRooks[3] == -1) { //if both rooks or king moved
			return possibleMoves;
		}

		uint16_t rookBeforeAndAfter; //the king moves a constant amount and can be found later

		//Includes king and rook in spots for extra safeguard to prevent castling while in check
		spotsToLeftRook = 0x1F;
		spotsToRightRook = 0xF0;

		if (castleRooks[2] != -1) { //if right rook, from white's perspective, has not moved or been captured yet
			if ((spotsToRightRook & ~squaresWhiteAttacks & (notCapturable ^ 144)) == spotsToRightRook) { //if no pieces attack castling squares
				rookBeforeAndAfter = 7;
				rookBeforeAndAfter |= (133 << 8);
				possibleMoves->push_back(rookBeforeAndAfter);
			}
		}
		//printBitBoard(spotsToRightRook & (~squaresWhiteAttacks | notCapturable));
		if (castleRooks[3] != -1) { //if left rook has not moved or been captured yet
			if ((spotsToLeftRook & ~squaresWhiteAttacks & (notCapturable ^ 17)) == spotsToLeftRook) { //if no pieces attack castling squares
				rookBeforeAndAfter = 0;
				rookBeforeAndAfter |= (131 << 8);
				possibleMoves->push_back(rookBeforeAndAfter);
			}
		}

		return possibleMoves;
	}
};

#endif