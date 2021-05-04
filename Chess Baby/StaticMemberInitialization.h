//Declares the static members of BoardInfo and TileWeights to reduce clutter in main.cpp
#include "BoardInfo.h"
#include "TileWeights.h"
char BoardInfo::pieceIndexBoard[64];
unsigned long long BoardInfo::bitBoard = 0ULL;
unsigned long long BoardInfo::whPieces = 0ULL;
unsigned long long BoardInfo::blPieces = 0ULL;

unsigned long long BoardInfo::whKing = 0ULL;
unsigned long long BoardInfo::whRook = 0ULL;
unsigned long long BoardInfo::whPawn = 0ULL;
unsigned long long BoardInfo::whQueen = 0ULL;
unsigned long long BoardInfo::whKnight = 0ULL;
unsigned long long BoardInfo::whBishop = 0ULL;
				   
unsigned long long BoardInfo::blKing = 0ULL;
unsigned long long BoardInfo::blRook = 0ULL;
unsigned long long BoardInfo::blPawn = 0ULL;
unsigned long long BoardInfo::blQueen = 0ULL;
unsigned long long BoardInfo::blKnight = 0ULL;
unsigned long long BoardInfo::blBishop = 0ULL;

unsigned long long BoardInfo::enPassantWhite = 0ULL; //spots white can enPassant
unsigned long long BoardInfo::enPassantBlack = 0ULL; //spots black can enPassant

unsigned long long BoardInfo::attackedSquares = 0ULL;
unsigned long long BoardInfo::squaresWhiteAttacks = 0ULL;
unsigned long long BoardInfo::squaresBlackAttacks = 0ULL;

unsigned long long BoardInfo::whAttackKing = 0ULL;
unsigned long long BoardInfo::whAttackRook = 0ULL;
unsigned long long BoardInfo::whAttackPawn = 0ULL;
unsigned long long BoardInfo::whAttackQueen = 0ULL;
unsigned long long BoardInfo::whAttackKnight = 0ULL;
unsigned long long BoardInfo::whAttackBishop = 0ULL;

unsigned long long BoardInfo::blAttackKing = 0ULL;
unsigned long long BoardInfo::blAttackRook = 0ULL;
unsigned long long BoardInfo::blAttackPawn = 0ULL;
unsigned long long BoardInfo::blAttackQueen = 0ULL;
unsigned long long BoardInfo::blAttackKnight = 0ULL;
unsigned long long BoardInfo::blAttackBishop = 0ULL;

unsigned long long BoardInfo::attackSquaresKing = 0ULL;
unsigned long long BoardInfo::attackSquaresRook = 0ULL;
unsigned long long BoardInfo::attackSquaresPawn = 0ULL;
unsigned long long BoardInfo::attackSquaresQueen = 0ULL;
unsigned long long BoardInfo::attackSquaresKnight = 0ULL;
unsigned long long BoardInfo::attackSquaresBishop = 0ULL;
unsigned long long BoardInfo::attackSquaresEnPassant = 0ULL;

unsigned long long BoardInfo::notCapturable = 0ULL;
unsigned long long BoardInfo::pinnedPiecesBitBoard = 0ULL;
unsigned long long BoardInfo::checkPathXRayThroughKing = 0ULL;
unsigned long long BoardInfo::enemyPiecesThatAreDefended = 0ULL;
unsigned long long BoardInfo::locationOfPieceAttackingKing = 0ULL;
unsigned long long BoardInfo::squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF;

int BoardInfo::castleRooks[4] = { 63, 56, 7, 0 };

int BoardInfo::numberOfChecks = 0;

const unsigned long long BoardInfo::KingSpan = 0x70507; //Centered on sq 9
const unsigned long long BoardInfo::KnightSpan = 0xA1100110A; //Centered on sq 18
const unsigned long long BoardInfo::FILE_A = 0x101010101010101ULL;
const unsigned long long BoardInfo::FILE_H = 0x8080808080808080ULL;
const unsigned long long BoardInfo::FILE_AB = 0x303030303030303ULL;
const unsigned long long BoardInfo::FILE_GH = 0xC0C0C0C0C0C0C0C0ULL;
const unsigned long long BoardInfo::RANK_1 = 0xFF00000000000000ULL;
const unsigned long long BoardInfo::RANK_4 = 0xFF00000000ULL;
const unsigned long long BoardInfo::RANK_5 = 0xFF000000ULL;
const unsigned long long BoardInfo::RANK_8 = 0xFFULL;
const unsigned long long BoardInfo::CENTRE = 0x1818000000ULL;
const unsigned long long BoardInfo::EXTENDED_CENTRE = 0x3C3C3C3C0000ULL;
const unsigned long long BoardInfo::KING_SIDE = 0xF0F0F0F0F0F0F0F0ULL;
const unsigned long long BoardInfo::QUEEN_SIDE = 0xF0F0F0F0F0F0F0FULL;

unsigned int BoardInfo::v = 0;
unsigned int BoardInfo::v2 = 0;

const unsigned long long BoardInfo::fileMasks[8] = //File A to file H
{ 0x101010101010101ULL, 0x202020202020202ULL, 0x404040404040404ULL, 0x808080808080808ULL,
  0x1010101010101010ULL, 0x2020202020202020ULL, 0x4040404040404040ULL, 0x8080808080808080ULL };
const unsigned long long BoardInfo::rankMasks[8] = //Rank 1 to rank 8
{ 0xFFULL, 0xFF00ULL, 0xFF0000ULL, 0xFF000000ULL, 0xFF00000000ULL, 0xFF0000000000ULL, 0xFF000000000000ULL, 0xFF00000000000000ULL };
const unsigned long long BoardInfo::diagonalMasks[15] = //Top left to bottom right
{ 0x1ULL, 0x102ULL, 0x10204ULL, 0x1020408ULL, 0x102040810ULL, 0x10204081020ULL, 0x1020408102040ULL, 0x102040810204080ULL,
	  0x204081020408000ULL, 0x408102040800000ULL, 0x810204080000000ULL, 0x1020408000000000ULL, 0x2040800000000000ULL,
	  0x4080000000000000ULL, 0x8000000000000000ULL };
const unsigned long long BoardInfo::antiDiagonalMasks[15] = //Top right to bottom left
{ 0x80ULL, 0x8040ULL, 0x804020ULL, 0x80402010ULL, 0x8040201008ULL, 0x804020100804ULL, 0x80402010080402ULL,
	  0x8040201008040201ULL, 0x4020100804020100ULL, 0x2010080402010000ULL, 0x1008040201000000ULL,
	  0x804020100000000ULL, 0x402010000000000ULL, 0x201000000000000ULL, 0x100000000000000ULL };

//opening
const short TileWeights::blKingWeightOp[64] = { -100, -200, -175, 0, 0, 0, -250, -20,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blRookWeightOp[64] = { 0, 0, -50, -50, -50, -50, 0, 0,
											    0, 0, -30, -30, -30, -30, 0, 0,
											    0, 0, -20, -20, -20, -20, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blPawnWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, -50, -50, -50,
												-10, -10, -10, -10, -10, -10, -20, -10,
												-20, -20, -25, -90, -90, -25, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blQueenWeightOp[64] = { 0, 0, 0, -20, -20, -20, 0, 0,
												 0, 0, -20, -20, -20, -20, 0, 0,
												 -20, -20, -20, -20, -20, -20, -20, -20,
												 0, 0, -10, -10, -10, -10, 0, 0,
												 0, 0, -10, -10, -10, -10, 0, 0,
												 0, 0, -10, -10, -10, -10, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blKnightWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 20, -10, -10, -10, -10, -10, -10, 20,
												 20, -20, -20, -20, -20, -20, -20, 20,
												 20, -30, -30, -30, -30, -30, -30, 20,
												 20, -30, -30, -30, -30, -30, -30, 20,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blBishopWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, -40, -20, -10, -10, -20, -40, 0,
												 0, -10, -20, -30, -30, -10, -10, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

/**********Black Pieces Opening************/
const short TileWeights::whKingWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    0, 0, 0, 0, 0, 0, 0, 0,
											    100, 200, 175, 0, 0, 0, 250, 20 };

const short TileWeights::whRookWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 20, 20, 20, 20, 0, 0,
												0, 0, 30, 30, 30, 30, 0, 0,
												5, 0, 50, 50, 50, 50, 0, 5 };

const short TileWeights::whPawnWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  20, 20, 20, 50, 50, 0, 0, 0,
											  20, 20, 20, 20, 0, 20, 0, 0,
											  0, 0, 0, 0, 0, 50, 50, 50,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whQueenWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 10, 10, 10, 10, 0, 0,
												0, 0, 10, 10, 10, 10, 0, 0,
												0, 0, 10, 10, 10, 10, 0, 0,
												0, 0, 20, 20, 20, 20, 0, 0,
												0, 0, 0, 20, 20, 20, 0, 0 };

const short TileWeights::whKnightWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 -20, 20, 20, 20, 20, 20, 20, -20,
												 -20, 10, 10, 10, 10, 10, 10, -20,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whBishopWeightOp[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 10, 20, 30, 30, 10, 10, 0,
												 0, 40, 20, 10, 10, 20, 40, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

/*******White Mid Game********/
const short TileWeights::whKingWeightMi[64] = { 100, 200, 175, 0, 0, 0, 250, 20,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whRookWeightMi[64] = { 5, 0, 50, 50, 50, 50, 0, 5,
												0, 0, 30, 30, 30, 30, 0, 0,
												0, 0, 20, 20, 20, 20, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whPawnWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 50, 50, 50,
											  10, 10, 10, 10, 10, 10, 20, 10,
											  20, 20, 25, 50, 50, 25, 0, 0,
											  20, 20, 20, 20, 20, 20, 20, 20,
											  20, 20, 20, 20, 20, 20, 20, 20,
											  30, 30, 30, 30, 30, 30, 30, 30,
											  90, 90, 90, 90, 90, 90, 90, 90 };

const short TileWeights::whQueenWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 20, 20, 20, 20, 0, 0,
												0, 20, 20, 20, 20, 20, 20, 0,
												0, 10, 20, 20, 20, 20, 10, 0,
												0, 10, 20, 20, 20, 20, 10, 0,
												0, 10, 20, 20, 20, 20, 10, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whKnightWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 20, 20, 20, 20, 20, 20, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whBishopWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 40, 20, 10, 10, 20, 40, 0,
												 0, 10, 20, 30, 30, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

/*********Black Mid Game************/
const short TileWeights::blKingWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, 0, 0, 0,
												-100, -200, -175, 0, 0, 0, -250, -20 };

const short TileWeights::blRookWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, -5, -5, -5, -5, 0, 0,
											  0, 0, -20, -20, -20, -20, 0, 0,
											  0, 0, -30, -30, -30, -30, 0, 0,
											  -5, 0, -50, -50, -50, -50, 0, -5 };

const short TileWeights::blPawnWeightMi[64] = { -90, -90, -90, -90, 0, 0, 0, 0,
											  0, -30, -30, -30, -30, -30, -30, 0,
											  0, -30, -30, -30, -30, -30, -30, 0,
											  0, -30, -30, -30, -30, -30, -30, 0,
											  -20, -20, -20, -50, -50, 0, 0, 0,
											  -20, -20, -20, -20, 0, -20, 0, 0,
											  0, 0, 0, 0, 0, -50, -50, -50,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blQueenWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											   0, 0, 0, 0, 0, 0, 0, 0,
											   0, -10, -10, -10, -10, -10, -10, 0,
											   0, -10, -10, -10, -10, -10, -10, 0,
											   0, -10, -10, -10, -10, -10, -10, 0,
											   0, -10, -10, -10, -10, -10, -10, 0,
											   0, -10, -20, -20, -20, -20, -10, 0,
											   0, 0, 0, -20, -20, -20, 0, 0 };

const short TileWeights::blKnightWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, -30, -30, -30, -30, -30, -30, 0,
												 0, -30, -30, -30, -30, -30, -30, 0,
												 0, -30, -30, -30, -30, -30, -30, 0,
												 0, -20, -20, -20, -20, -20, -20, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blBishopWeightMi[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 0, 0, 0, 0, 0, 0, 0,
												 0, -5, -5, -5, -5, -5, -5, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, -10, -20, -30, -30, -10, -10, 0,
												 0, -40, -20, -10, -10, -20, -40, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };


/***********White End Game***************/
const short TileWeights::whKingWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  0, 5, 5, 5, 5, 5, 5, 0,
											  0, 5, 5, 5, 5, 5, 5, 0,
											  0, 20, 20, 20, 20, 20, 20, 0,
											  0, 20, 20, 20, 20, 20, 20, 0,
											  0, 5, 5, 5, 5, 5, 5, 5,
											  0, 5, 5, 5, 5, 5, 5, 5,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whRookWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  5, 5, 5, 5, 5, 5, 5, 5,
											  5, 5, 5, 5, 5, 5, 5, 5,
											  5, 5, 5, 5, 5, 5, 5, 5,
											  5, 5, 5, 5, 5, 5, 5, 5,
											  5, 5, 5, 5, 5, 5, 5, 5,
											  5, 5, 5, 5, 5, 5, 5, 5,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whPawnWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 50, 50, 50,
											  10, 10, 10, 10, 10, 10, 20, 10,
											  20, 20, 25, 50, 50, 25, 0, 0,
											  20, 20, 20, 20, 20, 20, 20, 20,
											  20, 20, 20, 20, 20, 20, 20, 20,
											  30, 30, 30, 30, 30, 30, 30, 30,
											  90, 90, 90, 90, 90, 90, 90, 90 };

const short TileWeights::whQueenWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											   0, 0, 20, 20, 20, 20, 0, 0,
											   0, 20, 20, 20, 20, 20, 20, 0,
											   0, 10, 20, 20, 20, 20, 10, 0,
											   0, 10, 20, 20, 20, 20, 10, 0,
											   0, 10, 20, 20, 20, 20, 10, 0,
											   0, 0, 0, 0, 0, 0, 0, 0,
											   0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whKnightWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 20, 20, 20, 20, 20, 20, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 30, 30, 30, 30, 30, 30, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::whBishopWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, 10, 20, 10, 10, 20, 10, 0,
												 0, 10, 20, 30, 30, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 10, 10, 10, 10, 10, 10, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

/***********Black End Game***************/
const short TileWeights::blKingWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  0, -5, -5, -5, -5, -5, -5, -5,
											  0, -5, -5, -5, -5, -5, -5, -5,
											  0, -20, -20, -20, -20, -20, -20, 0,
											  0, -20, -20, -20, -20, -20, -20, 0,
											  0, -5, -5, -5, -5, -5, -5, -5,
											  0, -5, -5, -5, -5, -5, -5, -5,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blRookWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											  -5, -5, -5, -5, -5, -5, -5, -5,
											  -5, -5, -5, -5, -5, -5, -5, -5,
											  -5, -5, -5, -5, -5, -5, -5, -5,
											  -5, -5, -5, -5, -5, -5, -5, -5,
											  -5, -5, -5, -5, -5, -5, -5, -5,
											  -5, -5, -5, -5, -5, -5, -5, -5,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blPawnWeightEn[64] = { -90, -90, -90, -90, 0, 0, 0, 0,
											  0, -30, -30, -30, -30, -30, -30, 0,
											  0, -30, -30, -30, -30, -30, -30, 0,
											  0, -30, -30, -30, -30, -30, -30, 0,
											  -20, -20, -20, -50, -50, 0, 0, 0,
											  -20, -20, -20, -20, 0, -20, 0, 0,
											  0, 0, 0, 0, 0, -50, -50, -50,
											  0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blQueenWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
											   0, 0, -20, -20, -20, -20, 0, 0,
											   0, -20, -20, -20, -20, -20, -20, 0,
											   0, -10, -20, -20, -20, -20, -10, 0,
											   0, -10, -20, -20, -20, -20, -10, 0,
											   0, -10, -20, -20, -20, -20, -10, 0,
											   0, 0, -20, -20, -20, -20, 0, 0,
											   0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blKnightWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, -20, -20, -20, -20, -20, -20, 0,
												 0, -30, -30, -30, -30, -30, -30, 0,
												 0, -30, -30, -30, -30, -30, -30, 0,
												 0, -30, -30, -30, -30, -30, -30, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };

const short TileWeights::blBishopWeightEn[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, -10, -10, -10, -10, -10, -10, 0,
												 0, -20, -10, -10, -10, -10, -20, 0,
												 0, -20, -20, -10, -10, -10, -20, 0,
												 0, 0, 0, 0, 0, 0, 0, 0 };