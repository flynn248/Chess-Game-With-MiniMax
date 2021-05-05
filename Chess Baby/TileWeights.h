#pragma once
//Weights determine better piece placement for AI
struct TileWeights 	{
	//If few enemy pieces left, quarter off the board where king is at
	static const short firstQuadrant[64];
	static const short secndQuadrant[64];
	static const short thirdQuadrant[64];
	static const short forthQuadrant[64];
	static const short emptyQuadrant[64]; //an empty quadrant weight
	//opening
	static const short whKingWeightOp[64];
	static const short whRookWeightOp[64];
	static const short whPawnWeightOp[64];
	static const short whQueenWeightOp[64];
	static const short whKnightWeightOp[64];
	static const short whBishopWeightOp[64];
				 
	static const short blKingWeightOp[64];
	static const short blRookWeightOp[64];
	static const short blPawnWeightOp[64];
	static const short blQueenWeightOp[64];
	static const short blKnightWeightOp[64];
	static const short blBishopWeightOp[64];
				 
	//midgame	 
	static const short whKingWeightMi[64];
	static const short whRookWeightMi[64];
	static const short whPawnWeightMi[64];
	static const short whQueenWeightMi[64];
	static const short whKnightWeightMi[64];
	static const short whBishopWeightMi[64];
				 
	static const short blKingWeightMi[64];
	static const short blRookWeightMi[64];
	static const short blPawnWeightMi[64];
	static const short blQueenWeightMi[64];
	static const short blKnightWeightMi[64];
	static const short blBishopWeightMi[64];
				 
	//endgame	 
	static const short whKingWeightEn[64];
	static const short whRookWeightEn[64];
	static const short whPawnWeightEn[64];
	static const short whQueenWeightEn[64];
	static const short whKnightWeightEn[64];
	static const short whBishopWeightEn[64];
				 
	static const short blKingWeightEn[64];
	static const short blRookWeightEn[64];
	static const short blPawnWeightEn[64];
	static const short blQueenWeightEn[64];
	static const short blKnightWeightEn[64];
	static const short blBishopWeightEn[64];

};