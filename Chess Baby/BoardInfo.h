#ifndef BOARDINFO_H
#define BOARDINFO_H
int revCount = 0;

struct BoardInfo {
private: //frequently used variables in below functions.
	static unsigned int v, v2;
	static int d;
public:
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
	static unsigned char moveNumber;

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

	//Not needed in Saved Game State
	static unsigned long long movedPieceHighlightLoc; //see where the last piece was moved from
	static char pieceIndexBoard[64]; //dead variable

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

		for (int i = 0; i < 64; i++) 		{
			pieceIndexBoard[i] = -1;
		}
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

	static unsigned long long reverseBits(unsigned long long origBits) { //reverse the order of the bits
		revCount++;
		/*
			Faster bit swapping techniques was found here.
			http://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
			Stackoverflow page where the above link was found. Other potentially faster methods discussed in the link
			https://stackoverflow.com/questions/746171/efficient-algorithm-for-bit-reversal-from-msb-lsb-to-lsb-msb-in-c
		*/

		//Process was broken into two parts for each half of the 64-bit variable
		v = (unsigned int)origBits; // 32-bit word to reverse bit order

		// swap odd and even bits
		v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
		// swap consecutive pairs
		v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
		// swap nibbles ... 
		v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
		// swap bytes
		v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
		// swap 2-byte long pairs
		v = (v >> 16) | (v << 16);

		v2 = origBits >> 32; //now doing back 32 bits

		v2 = ((v2 >> 1) & 0x55555555) | ((v2 & 0x55555555) << 1);
		v2 = ((v2 >> 2) & 0x33333333) | ((v2 & 0x33333333) << 2);
		v2 = ((v2 >> 4) & 0x0F0F0F0F) | ((v2 & 0x0F0F0F0F) << 4);
		v2 = ((v2 >> 8) & 0x00FF00FF) | ((v2 & 0x00FF00FF) << 8);
		v2 = (v2 >> 16) | (v2 << 16);

		origBits = ((unsigned long long)v) << 32;
		origBits |= v2;
		return origBits;
	}

	static int numOfTrailingZeros(unsigned long long bitMap) { //potentially change this idea to instead keep a vector of each piece location on their bitMap
		//binary search method
		// d will be the number of zero bits on the right

		if (bitMap & 0x1) { //bitMap starts with a 1
			return 0;
		}
		else {
			d = 1;
			if ((bitMap & 0xffffffff) == 0) {
				bitMap >>= 32;
				d += 32;
			}
			if ((bitMap & 0xffff) == 0) {
				bitMap >>= 16;
				d += 16;
			}
			if ((bitMap & 0xff) == 0) {
				bitMap >>= 8;
				d += 8;
			}
			if ((bitMap & 0xf) == 0) {
				bitMap >>= 4;
				d += 4;
			}
			if ((bitMap & 0x3) == 0) {
				bitMap >>= 2;
				d += 2;
			}
			d -= bitMap & 0x1;
		}

		return d;
	}
};
#endif // !BOARDINFO_H




