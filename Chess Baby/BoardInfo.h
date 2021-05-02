#ifndef BOARDINFO_H
#define BOARDINFO_H

struct BoardInfo {
	static unsigned long long bitBoard;
	static unsigned long long whPieces;
	static unsigned long long blPieces;

	static unsigned long long whKing;
	static unsigned long long whRook;
	static unsigned long long whPawn;
	static unsigned long long whQueen;
	static unsigned long long whKnight;
	static unsigned long long whBishop;
									  
	static unsigned long long blKing;
	static unsigned long long blRook;
	static unsigned long long blPawn;
	static unsigned long long blQueen;
	static unsigned long long blKnight;
	static unsigned long long blBishop;

	static unsigned long long enPassantWhite; //spots white can enPassant
	static unsigned long long enPassantBlack; //spots black can enPassant

	static unsigned long long attackedSquares;
	static unsigned long long squaresWhiteAttacks;
	static unsigned long long squaresBlackAttacks;
											 
	static unsigned long long whAttackKing;
	static unsigned long long whAttackRook;
	static unsigned long long whAttackPawn;
	static unsigned long long whAttackQueen;
	static unsigned long long whAttackKnight;
	static unsigned long long whAttackBishop;
											
	static unsigned long long blAttackKing;
	static unsigned long long blAttackRook;
	static unsigned long long blAttackPawn;
	static unsigned long long blAttackQueen;
	static unsigned long long blAttackKnight;
	static unsigned long long blAttackBishop;

	static unsigned long long attackSquaresKing;
	static unsigned long long attackSquaresRook;
	static unsigned long long attackSquaresPawn;
	static unsigned long long attackSquaresQueen;
	static unsigned long long attackSquaresKnight;
	static unsigned long long attackSquaresBishop;
	static unsigned long long attackSquaresEnPassant;

	static unsigned long long notCapturable;
	static unsigned long long pinnedPiecesBitBoard;
	static unsigned long long checkPathXRayThroughKing;
	static unsigned long long enemyPiecesThatAreDefended;
	static unsigned long long locationOfPieceAttackingKing;
	static unsigned long long squaresToBlockCheckOrCapture;

	static int castleRooks[4];

	static int numberOfChecks;

	static const unsigned long long KingSpan;
	static const unsigned long long KnightSpan;
	static const unsigned long long FILE_A;
	static const unsigned long long FILE_H;
	static const unsigned long long FILE_AB;
	static const unsigned long long FILE_GH;
	static const unsigned long long RANK_1;
	static const unsigned long long RANK_4;
	static const unsigned long long RANK_5;
	static const unsigned long long RANK_8;
	static const unsigned long long CENTRE;
	static const unsigned long long EXTENDED_CENTRE;
	static const unsigned long long KING_SIDE;
	static const unsigned long long QUEEN_SIDE;

	static const unsigned long long fileMasks[8]; //File A to file H
	static const unsigned long long rankMasks[8]; //Rank 1 to rank 8
	static const unsigned long long diagonalMasks[15]; //Top left to bottom right
	static const unsigned long long antiDiagonalMasks[15]; //Top right to bottom left

	static void setStartPosition() { //Set start position of pieces on the bitBoard
	//Index of new bit defined by (row * 8 + col)
		whPawn |= (0b11111111ULL << 48);
		whRook |= (0b10000001ULL << 56);
		whBishop |= (0b1001ULL << 58);
		whKnight |= (0b100001ULL << 57);
		whKing |= (0b1ULL << 60);
		whQueen |= (0b1ULL << 59);

		blPawn |= (0b11111111ULL << 8);
		blRook |= (0b10000001ULL);
		blBishop |= (0b1001ULL << 2);
		blKnight |= (0b100001ULL << 1);
		blKing |= (0b1ULL << 4);
		blQueen |= (0b1ULL << 3);

		bitBoard |= whPawn;
		bitBoard |= whRook;
		bitBoard |= whBishop;
		bitBoard |= whKnight;
		bitBoard |= whKing;
		bitBoard |= whQueen;
		whPieces |= bitBoard;

		bitBoard |= blPawn;
		bitBoard |= blRook;
		bitBoard |= blBishop;
		bitBoard |= blKnight;
		bitBoard |= blKing;
		bitBoard |= blQueen;
		blPieces |= bitBoard ^ whPieces;
	}
	static unsigned long long HorzNVerticalMoves(const int& square) { //Returns all horizontal and vertical moves for a piece
		unsigned long long binSqare = 1ULL << square;
		unsigned long long horizontalMoves = (bitBoard - 2 * binSqare) ^ reverseBits(reverseBits(bitBoard) - 2 * reverseBits(binSqare));
		unsigned long long verticalMoves = ((bitBoard & fileMasks[square % 8]) - (2 * binSqare)) ^
			reverseBits(reverseBits(bitBoard & fileMasks[square % 8]) - (2 * reverseBits(binSqare)));

		return (horizontalMoves & rankMasks[square / 8]) | (verticalMoves & fileMasks[square % 8]);
	}

	static unsigned long long diagNAntiDagMoves(const int& square) {//Returns all diagonal and antiDiagonal moves for a piece
		unsigned long long binSquare = 1ULL << square;
		unsigned long long diagonalMoves = ((bitBoard & diagonalMasks[(square / 8) + (square % 8)]) - (2 * binSquare)) ^
			reverseBits(reverseBits(bitBoard & diagonalMasks[(square / 8) + (square % 8)]) - (2 * reverseBits(binSquare)));

		unsigned long long antiDiagonalMoves = ((bitBoard & antiDiagonalMasks[(square / 8) + 7 - (square % 8)]) - (2 * binSquare) ^
			reverseBits(reverseBits(bitBoard & antiDiagonalMasks[(square / 8) + 7 - (square % 8)]) - (2 * reverseBits(binSquare))));

		return (diagonalMoves & diagonalMasks[(square / 8) + (square % 8)] | antiDiagonalMoves & antiDiagonalMasks[(square / 8) + 7 - (square % 8)]);
	}

	static unsigned long long diagonalMoves(const int& square) {
		unsigned long long binSquare = 1ULL << square;
		unsigned long long diagonalMoves = ((bitBoard & diagonalMasks[(square / 8) + (square % 8)]) - (2 * binSquare)) ^
			reverseBits(reverseBits(bitBoard & diagonalMasks[(square / 8) + (square % 8)]) - (2 * reverseBits(binSquare)));
		return (diagonalMoves & diagonalMasks[(square / 8) + (square % 8)]);
	}
	static unsigned long long antiDiagMoves(const int& square) {
		unsigned long long binSquare = 1ULL << square;
		unsigned long long antiDiagonalMoves = ((bitBoard & antiDiagonalMasks[(square / 8) + 7 - (square % 8)]) - (2 * binSquare) ^
			reverseBits(reverseBits(bitBoard & antiDiagonalMasks[(square / 8) + 7 - (square % 8)]) - (2 * reverseBits(binSquare))));
		return  (antiDiagonalMoves & antiDiagonalMasks[(square / 8) + 7 - (square % 8)]);
	}
	static unsigned long long horizontalMoves(const int& square) {
		unsigned long long binSqare = 1ULL << square;
		unsigned long long horizontalMoves = (bitBoard - 2 * binSqare) ^ reverseBits(reverseBits(bitBoard) - 2 * reverseBits(binSqare));
		return (horizontalMoves & rankMasks[square / 8]);
	}
	static unsigned long long verticalMoves(const int& square) {
		unsigned long long binSqare = 1ULL << square;
		unsigned long long verticalMoves = ((bitBoard & fileMasks[square % 8]) - (2 * binSqare)) ^
			reverseBits(reverseBits(bitBoard & fileMasks[square % 8]) - (2 * reverseBits(binSqare)));
		return (verticalMoves & fileMasks[square % 8]);
	}
	static unsigned long long reverseBits(const unsigned long long& origBits) { //reverse the order of the bits
		unsigned long long reversed = 0ULL;

		//for (int i = 0; i < 32; i++) {
		//	reversed |= ((origBits >> i) & 0b1) << (63 - i);
		//	reversed |= ((origBits >> (63 - i)) & 0b1) << i;
		//}

		for (int i = 0; i < 64; i++) { //fastest method so far
			reversed |= ((origBits >> i) & 0b1) << (63 - i);
		}


		//std::cout << "Reversed: " << std::bitset<64>(reversed) << std::endl;
		return reversed;
	}

	static int numOfTrailingZeros(unsigned long long bitMap) { //potentially change this idea to instead keep a vector of each piece location on their bitMap
				//binary search method
		unsigned long long v = 0ULL;
		v |= bitMap;

		int d;     // d will be the number of zero bits on the right,
							// so if v is 1101000 (base 2), then d will be 3
		// NOTE: if 0 == v, then d = 31.
		if (v & 0x1) {
			// special dase for odd v (assumed to happen half of the time)
			return 0;
		}
		else {
			d = 1;
			if ((v & 0xffffffff) == 0) {
				v >>= 32;
				d += 32;
			}
			if ((v & 0xffff) == 0) {
				v >>= 16;
				d += 16;
			}
			if ((v & 0xff) == 0) {
				v >>= 8;
				d += 8;
			}
			if ((v & 0xf) == 0) {
				v >>= 4;
				d += 4;
			}
			if ((v & 0x3) == 0) {
				v >>= 2;
				d += 2;
			}
			d -= v & 0x1;
		}


		return d;
	}
};
#endif // !BOARDINFO_H

	//static unsigned long long whPinnedPiecesBitBoard; //not used
	//static unsigned long long blPinnedPiecesBitBoard; //not used
	//static unsigned long long pawnPinnedLegalMoves; //not used
	//static unsigned long long queenPinnedLegalMoves; //not used
	//static unsigned long long bishopPinnedLegalMoves; //not used
	//static unsigned long long rookPinnedLegalMoves; //not used