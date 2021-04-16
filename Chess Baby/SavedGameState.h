#include <vector>
#include <memory>
#ifndef GAMESTATE_H
#define GAMESTATE_H

struct SavedGameState {
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<uint16_t>>>> allMoves;


	SavedGameState() {
		allMoves = std::make_unique<std::vector<std::unique_ptr<std::vector<uint16_t>>>>();
		allMoves->resize(8);
	}
};

#endif // !GAMESTATE_H

	//unsigned long long cpyBitBoard = 0ULL;
	//unsigned long long cpyWhPieces = 0ULL;
	//unsigned long long cpyBlPieces = 0ULL;
	//
	//unsigned long long cpyWhKing = 0ULL;
	//unsigned long long cpyWhRook = 0ULL;
	//unsigned long long cpyWhPawn = 0ULL;
	//unsigned long long cpyWhQueen = 0ULL;
	//unsigned long long cpyWhKnight = 0ULL;
	//unsigned long long cpyWhBishop = 0ULL;
	//
	//unsigned long long cpyBlKing = 0ULL;
	//unsigned long long cpyBlRook = 0ULL;
	//unsigned long long cpyBlPawn = 0ULL;
	//unsigned long long cpyBlQueen = 0ULL;
	//unsigned long long cpyBlKnight = 0ULL;
	//unsigned long long cpyBlBishop = 0ULL;
	//
	//unsigned long long cpyWhAttackKing = 0ULL;
	//unsigned long long cpyWhAttackRook = 0ULL;
	//unsigned long long cpyWhAttackPawn = 0ULL;
	//unsigned long long cpyWhAttackQueen = 0ULL;
	//unsigned long long cpyWhAttackKnight = 0ULL;
	//unsigned long long cpyWhAttackBishop = 0ULL;
	//
	//unsigned long long cpyBlAttackKing = 0ULL;
	//unsigned long long cpyBlAttackRook = 0ULL;
	//unsigned long long cpyBlAttackPawn = 0ULL;
	//unsigned long long cpyBlAttackQueen = 0ULL;
	//unsigned long long cpyBlAttackKnight = 0ULL;
	//unsigned long long cpyBlAttackBishop = 0ULL;
	//
	//unsigned long long cpySquaresToBlockCheckOrCapture = 0ULL;
	//unsigned long long cpyLocationOfPieceAttackingKing = 0ULL;
	//unsigned long long cpyPinnedPiecesBitBoard = 0ULL;
	//unsigned long long cpyCheckPathXRayThroughKing = 0ULL;
	//unsigned long long cpyEnemyPiecesThatAreDefended = 0ULL;
	//unsigned long long cpyNotCapturable = 0ULL;
	//
	//int cpyNumberOfChecks = 0;