#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <bitset>
#include <vector>
#include <memory>
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "Timer.h"
#include "BoardInfo.h"
#include "TileWeights.h"
//#include "Stack.h"

class ChessBoard : public BoardInfo, private TileWeights{
protected:
	static const int BSIZE = 8;
	float tileWidth;
	bool isWhiteMove; //true for white move, false for black
	bool isCheckMate = false,
		isStaleMate = false;
	int timesWhiteKingMoved = 0, //act like a semaphore for the black king movement. Once king is committed to moving, it cannot castle again
		whenWhRightRookMoved = 0,
		whenWhLeftRookMoved = 0,
		timesBlackKingMoved = 0,
		whenBlRightRookMoved = 0,
		whenBlLeftRookMoved = 0;
	int whMoveCounter = 1,
		blMoveCounter = 1;

	sf::RectangleShape board[BSIZE][BSIZE];
	sf::RectangleShape tileSquare;
	sf::Color whtTileColor,
		blkTileColor;

	King* whKingPiece = nullptr;
	Pawn* whPawnPiece = nullptr;
	Rook* whRookPiece = nullptr;
	Queen* whQueenPiece = nullptr;
	Knight* whKnightPiece = nullptr;
	Bishop* whBishopPiece = nullptr;

	King* blKingPiece = nullptr;
	Pawn* blPawnPiece = nullptr;
	Rook* blRookPiece = nullptr;
	Queen* blQueenPiece = nullptr;
	Knight* blKnightPiece = nullptr;
	Bishop* blBishopPiece = nullptr;

	//std::unique_ptr<Stack> capturedPiecesAI;
	void customStartPosition() {
		whKing = 0;
		whRook = 0;
		whPawn = 0;
		whQueen = 0;
		whKnight = 0;
		whBishop = 0;

		blKing = 0;
		blRook = 0;
		blPawn = 0;
		blQueen = 0;
		blKnight = 0;
		blBishop = 0;
		//whPawn |= (0b11111111ULL << 48);
		//whRook |= (0b10000001ULL << 56);
		//whBishop |= (0b1001ULL << 58);
		//whKnight |= (0b100001ULL << 57);
		whKing |= (0b1ULL << 60);
		//whQueen |= (0b1ULL << 59);

		blPawn |= (0b1ULL << 48);
		blRook |= (0b10000001ULL);
		//blBishop |= (0b1001ULL << 2);
		//blKnight |= (0b100001ULL << 1);
		blKing |= (0b1ULL << 4);
		//blQueen |= (0b1ULL << 3);

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

public:											  												  
	ChessBoard(const float &tileWidth, sf::Color & whiteCol, sf::Color & blkCol, const float &frstSquareCentRef, const int& stackSize) {
		this->tileWidth = tileWidth;
		whtTileColor = whiteCol;
		blkTileColor = blkCol;
		isWhiteMove = true;
		//capturedPiecesAI = std::make_unique<Stack>(stackSize);
		initializePiecePosition(frstSquareCentRef);
	}
												  
	~ChessBoard() {
		delete whRookPiece;
		delete whKnightPiece;
		delete whBishopPiece;
		delete whQueenPiece;
		delete whKingPiece;
		delete whPawnPiece;

		delete blRookPiece;
		delete blKnightPiece;
		delete blBishopPiece;
		delete blQueenPiece;
		delete blKingPiece;
		delete blPawnPiece;

		whKingPiece = nullptr;
		whPawnPiece = nullptr;
		whRookPiece = nullptr;
		whQueenPiece = nullptr;
		whKnightPiece = nullptr;
		whBishopPiece = nullptr;

		blKingPiece = nullptr;
		blPawnPiece = nullptr;
		blRookPiece = nullptr;
		blQueenPiece = nullptr;
		blKnightPiece = nullptr;
		blBishopPiece = nullptr;
	}						  
												  
	void initializePiecePosition(const float& frstSquareCentRef) {
		
		setStartPosition();

		const float pieceScale = (1 - (tileWidth / (tileWidth * 8))) * 0.40f;
		float whiteYOffset = frstSquareCentRef + (tileWidth  * 7);

		const int kingVal = 10000,
			queenVal = 900,
			rookVal = 500,
			knightVal = 300,
			bishopVal = 300,
			pawnVal = 100;
		
		whRookPiece = new Rook(rookVal, pieceScale, "assets/white-rook.png", frstSquareCentRef, true, "R", whRook, 2);
		whKnightPiece = new Knight(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef, true, "N", whKnight, 2);
		whBishopPiece = new Bishop(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef, true, "B", whBishop, 2);
		whQueenPiece = new Queen(queenVal, pieceScale, "assets/white-queen.png", frstSquareCentRef, true, "Q", whQueen,	1);
		whKingPiece = new King(kingVal, pieceScale, "assets/white-king.png", frstSquareCentRef, true, "K", whKing, 1);
		whPawnPiece = new Pawn(pawnVal, pieceScale, "assets/white-pawn.png", frstSquareCentRef, true, "P", whPawn, 8);

		blRookPiece = new Rook(rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef, false, "r", blRook, 2);
		blKnightPiece = new Knight(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef, false, "n", blKnight, 2);
		blBishopPiece = new Bishop(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef, false, "b", blBishop, 2);
		blQueenPiece = new Queen(queenVal * -1, pieceScale, "assets/black-queen.png", frstSquareCentRef, false, "q", blQueen, 1);
		blKingPiece = new King(kingVal * -1, pieceScale, "assets/black-king.png", frstSquareCentRef, false, "k", blKing, 1);
		blPawnPiece = new Pawn(pawnVal * -1, pieceScale, "assets/black-pawn.png", frstSquareCentRef, false, "p", blPawn, 8);
	}

	void drawBoard(sf::RenderWindow& window) {
		tileSquare.setSize(sf::Vector2f(tileWidth, tileWidth));

		for (int i = 0; i < BSIZE; i++) {
			for (int j = 0; j < BSIZE; j++) {
				board[i][j] = tileSquare;
				tileSquare.setPosition(sf::Vector2f(tileWidth * i, tileWidth * j));
				if ((i + j) % 2) {
					tileSquare.setFillColor(blkTileColor);
				}
				else {

					tileSquare.setFillColor(whtTileColor);
				}
				window.draw(board[i][j]);
			}
		}

		drawPieces(window);
	}
	void drawPieces(sf::RenderWindow& window) {
		whKingPiece->printSprite(window);
		whPawnPiece->printSprite(window);
		whRookPiece->printSprite(window);
		whQueenPiece->printSprite(window);
		whKnightPiece->printSprite(window);
		whBishopPiece->printSprite(window);

		blKingPiece->printSprite(window);
		blPawnPiece->printSprite(window);
		blRookPiece->printSprite(window);
		blQueenPiece->printSprite(window);
		blKnightPiece->printSprite(window);
		blBishopPiece->printSprite(window);
	}
	void printBitBoard(const unsigned long long & aBitBoard) {

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				std::cout << std::bitset<1>(aBitBoard >> (i*8 + j)) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	void printPiecesCount() {
		std::cout << "Piece Counts:" << std::endl;

		std::cout << std::setw(6) << " " << "Kg Qn Bh Ro Kn Pn\n"
			<< "White  "
			<< whKingPiece->getNumPieces() << "  "
			<< whQueenPiece->getNumPieces() << "  "
			<< whBishopPiece->getNumPieces() << "  "
			<< whRookPiece->getNumPieces() << "  "
			<< whKnightPiece->getNumPieces() << "  "
			<< whPawnPiece->getNumPieces() << std::endl
			<< "Black  "
			<< blKingPiece->getNumPieces() << "  "
			<< blQueenPiece->getNumPieces() << "  "
			<< blBishopPiece->getNumPieces() << "  "
			<< blRookPiece->getNumPieces() << "  "
			<< blKnightPiece->getNumPieces() << "  "
			<< blPawnPiece->getNumPieces() << std::endl;
	}
	void printPiecesLocVectSize() {
		std::cout << "Piece Location Vector Size:" << std::endl;

		std::cout << std::setw(6) << " " << "Kg Qn Bh Ro Kn Pn\n"
			<< "White  "
			<< whKingPiece->getLocVectSize() << "  "
			<< whQueenPiece->getLocVectSize() << "  "
			<< whBishopPiece->getLocVectSize() << "  "
			<< whRookPiece->getLocVectSize() << "  "
			<< whKnightPiece->getLocVectSize() << "  "
			<< whPawnPiece->getLocVectSize() << std::endl
			<< "Black  "
			<< blKingPiece->getLocVectSize() << "  "
			<< blQueenPiece->getLocVectSize() << "  "
			<< blBishopPiece->getLocVectSize() << "  "
			<< blRookPiece->getLocVectSize() << "  "
			<< blKnightPiece->getLocVectSize() << "  "
			<< blPawnPiece->getLocVectSize() << std::endl;
	}
	bool isPieceHere(const int& x, const int& y) {
		int squareIndex = (y * 8) + x;
		if ((bitBoard & (1ULL << squareIndex)) != 0) {
			return true;
		}
		return false;
	}

	int evaluateBoardValue(const bool& isItWhiteMove) {
		int whitePiecesValue = 0,
			blackPiecesValue = 0;

		whitePiecesValue += whKingPiece->getNumPieces() * whKingPiece->getValue();
		whitePiecesValue += whPawnPiece->getNumPieces() * whPawnPiece->getValue();
		whitePiecesValue += whRookPiece->getNumPieces() * whRookPiece->getValue();
		whitePiecesValue += whQueenPiece->getNumPieces() * whQueenPiece->getValue();
		whitePiecesValue += whKnightPiece->getNumPieces() * whKnightPiece->getValue();
		whitePiecesValue += whBishopPiece->getNumPieces() * whBishopPiece->getValue();
		
		blackPiecesValue += blKingPiece->getNumPieces() * blKingPiece->getValue();
		blackPiecesValue += blPawnPiece->getNumPieces() * blPawnPiece->getValue();
		blackPiecesValue += blRookPiece->getNumPieces() * blRookPiece->getValue();
		blackPiecesValue += blQueenPiece->getNumPieces() * blQueenPiece->getValue();
		blackPiecesValue += blKnightPiece->getNumPieces() * blKnightPiece->getValue();
		blackPiecesValue += blBishopPiece->getNumPieces() * blBishopPiece->getValue();


		evaluatePlacementWhite(whKing, whitePiecesValue, whKingWeightOp);
		evaluatePlacementWhite(whPawn, whitePiecesValue, whPawnWeightOp);
		evaluatePlacementWhite(whRook, whitePiecesValue, whRookWeightOp);
		evaluatePlacementWhite(whQueen, whitePiecesValue, whQueenWeightOp);
		evaluatePlacementWhite(whKnight, whitePiecesValue, whKnightWeightOp);

		evaluatePlacementBlack(blKing, blackPiecesValue, blKingWeightOp);
		evaluatePlacementBlack(blPawn, blackPiecesValue, blPawnWeightOp);
		evaluatePlacementBlack(blRook, blackPiecesValue, blRookWeightOp);
		evaluatePlacementBlack(blQueen, blackPiecesValue, blQueenWeightOp);
		evaluatePlacementBlack(blKnight, blackPiecesValue, blKnightWeightOp);
		evaluatePlacementBlack(blBishop, blackPiecesValue, blBishopWeightOp);


		//Add values for check, checkmate, and stalemate.
		//Check = + 100
		//Checkmate = + 99999
		//Stalemate = * 0
		return whitePiecesValue + blackPiecesValue;
	}

private: //stuff for AI only
	void evaluatePlacementWhite(unsigned long long pieceBitBoard, int& piecesValue, const short* pieceWeight) {
		unsigned char pieceIndex;
		while (pieceBitBoard != 0) 	{
			pieceIndex = numOfTrailingZeros(pieceBitBoard);
			piecesValue += pieceWeight[pieceIndex];
			pieceBitBoard &= ~(1ULL << pieceIndex);
		}
	}
	void evaluatePlacementBlack(unsigned long long pieceBitBoard, int& piecesValue, const short* pieceWeight) {
		unsigned char pieceIndex;
		while (pieceBitBoard != 0) {
			pieceIndex = numOfTrailingZeros(pieceBitBoard);
			piecesValue += pieceWeight[pieceIndex];
			pieceBitBoard &= ~(1ULL << pieceIndex);
		}
	}

	void makeAIMoveWhite(uint16_t& beforeNAfterMove, const int& currDepth) {
		unsigned char newTileIndex = (beforeNAfterMove & 16128) >> 8;
		unsigned char initialTileIndex = beforeNAfterMove & 63;

		if (((1ULL << newTileIndex) & blPieces) != 0) { //if captured a piece
			try {
				if (((1ULL << newTileIndex) & blPawn) != 0) {
					blPawnPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(blPawnPiece, currDepth)
				}
				else if (((1ULL << newTileIndex) & blBishop) != 0) {
					blBishopPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(blBishopPiece, currDepth);
				}
				else if (((1ULL << newTileIndex) & blRook) != 0) {
					blRookPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(blRookPiece, currDepth);
					if ((blRook & 128) != 128) { //if right rook moved
						castleRooks[2] = -1;
						if (whenBlRightRookMoved == 0)
							whenBlRightRookMoved = whMoveCounter;
					}
					if ((blRook & 1) != 1) { //if left rook moved
						castleRooks[3] = -1;

						if (whenBlLeftRookMoved == 0)
							whenBlLeftRookMoved = whMoveCounter;
					}
				}
				else if (((1ULL << newTileIndex) & blQueen) != 0) {
					blQueenPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(blQueenPiece, currDepth);
				}
				else if (((1ULL << newTileIndex) & blKnight) != 0) {
					blKnightPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(blKnightPiece, currDepth);
				}
				else
					throw std::logic_error("Error in finding the captured piece\n");

			}
			catch (const std::exception& e) {
				std::cout << e.what();
			}
		}

		if ((beforeNAfterMove & 49344) == 0) { //if no special moves
			makeMoveWhitePieceAI(initialTileIndex, newTileIndex);
		}
		else if ((beforeNAfterMove & 64) == 64) { //Pawn promotion
			whPawnPiece->removePieceBitBoard(initialTileIndex);
			addWhiteQueenToBoard(newTileIndex);
		}
		else if ((beforeNAfterMove & 128) == 128) { //pawn moved two squares
			whPawnPiece->removePieceBitBoard(initialTileIndex);
			whPawnPiece->addPieceBitBoard(newTileIndex);
			enPassantBlack |= (1ULL << (newTileIndex + 8));
		}
		else if ((beforeNAfterMove & 16384) == 16384) { //if enpassant
			blPawnPiece->removePieceBitBoard(newTileIndex + 8);
			whPawnPiece->removePieceBitBoard(initialTileIndex);
			whPawnPiece->addPieceBitBoard(newTileIndex);

			//capturedPiecesAI->pushEnPassantBoard(enPassantWhite);
		}
		else { //castling
			if (initialTileIndex == 63) //castled king side
				whKing = 1ULL << 62;
			else
				whKing = 1ULL << 58;

			timesWhiteKingMoved++;
			whKingPiece->updateHasKingMoved();
			whRookPiece->removePieceBitBoard(initialTileIndex);
			whRookPiece->addPieceBitBoard(newTileIndex);
		}
		whMoveCounter++;
		updateBitBoard();
		updateSquaresWhiteAttacks();
	}
	void makeAIMoveBlack(uint16_t& beforeNAfterMove, const int& currDepth) {
		unsigned char newTileIndex = (beforeNAfterMove & 16128) >> 8;
		unsigned char initialTileIndex = beforeNAfterMove & 63;

		if (((1ULL << newTileIndex) & whPieces) != 0) { //if captured a piece
			try {
				if (((1ULL << newTileIndex) & whPawn) != 0) {
					whPawnPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(whPawnPiece, currDepth);
				}
				else if (((1ULL << newTileIndex) & whBishop) != 0) {
					whBishopPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(whBishopPiece, currDepth);
				}
				else if (((1ULL << newTileIndex) & whRook) != 0) {
					whRookPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(whRookPiece, currDepth);
					if ((whRook & 0x8000000000000000) != 0x8000000000000000) { //if right rook moved
						castleRooks[0] = -1;
						if (whenWhRightRookMoved == 0)
							whenWhRightRookMoved = blMoveCounter;
					}
					if ((whRook & 0x100000000000000) != 0x100000000000000) { //if left rook moved
						castleRooks[1] = -1;
						if (whenWhLeftRookMoved == 0)
							whenWhLeftRookMoved = blMoveCounter;
					}
				}
				else if (((1ULL << newTileIndex) & whQueen) != 0) {
					whQueenPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(whQueenPiece, currDepth);
				}
				else if (((1ULL << newTileIndex) & whKnight) != 0) {
					whKnightPiece->removePieceBitBoard(newTileIndex);
					//capturedPiecesAI->push(whKnightPiece, currDepth);
				}
				else
					throw std::logic_error("Error in finding the captured piece\n");
			}
			catch (const std::exception& e) {
				std::cout << e.what();
			}
		}

		if ((beforeNAfterMove & 49344) == 0) { //if no special moves
			makeMoveBlackPieceAI(initialTileIndex, newTileIndex);
		}
		else if ((beforeNAfterMove & 64) == 64) { //Pawn promotion
			blPawnPiece->removePieceBitBoard(initialTileIndex);
			addBlackQueenToBoard(newTileIndex);
		}
		else if ((beforeNAfterMove & 128) == 128) { //pawn moved two squares
			blPawnPiece->removePieceBitBoard(initialTileIndex);
			blPawnPiece->addPieceBitBoard(newTileIndex);
			enPassantWhite |= (1ULL << (newTileIndex - 8));
		}
		else if ((beforeNAfterMove & 16384) == 16384) { //if enpassant
			whPawnPiece->removePieceBitBoard(newTileIndex - 8);
			blPawnPiece->removePieceBitBoard(initialTileIndex);
			blPawnPiece->addPieceBitBoard(newTileIndex);
		}
		else { //castling
			if (initialTileIndex == 7) //castled king side
				blKing = 1ULL << 6;
			else
				blKing = 1ULL << 2;

			timesBlackKingMoved++;
			blKingPiece->updateHasKingMoved();
			blRookPiece->removePieceBitBoard(initialTileIndex);
			blRookPiece->addPieceBitBoard(newTileIndex);
		}
		
		updateBitBoard();
		updateSquaresBlackAttacks();
	}

	void makeMoveWhitePieceAI(const int& initialTileIndex, const int& newTileIndex) {
		if (((1ULL << initialTileIndex) & whKing) != 0) {
			timesWhiteKingMoved++;
			whKingPiece->removePieceBitBoard(initialTileIndex);
			whKingPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & whPawn) != 0) {
			whPawnPiece->removePieceBitBoard(initialTileIndex);
			whPawnPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & whBishop) != 0) {
			whBishopPiece->removePieceBitBoard(initialTileIndex);
			whBishopPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & whRook) != 0) {
			whRookPiece->removePieceBitBoard(initialTileIndex);
			whRookPiece->addPieceBitBoard(newTileIndex);
			if ((whRook & 0x8000000000000000) != 0x8000000000000000) { //if right rook moved
				castleRooks[0] = -1;
				if (whenWhRightRookMoved == 0)
					whenWhRightRookMoved = whMoveCounter;
			}
			if ((whRook & 0x100000000000000) != 0x100000000000000) { //if left rook moved
				castleRooks[1] = -1;
				if (whenWhLeftRookMoved == 0)
					whenWhLeftRookMoved = whMoveCounter;
			}
		}
		else if (((1ULL << initialTileIndex) & whQueen) != 0) {
			whQueenPiece->removePieceBitBoard(initialTileIndex);
			whQueenPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & whKnight) != 0) {
			whKnightPiece->removePieceBitBoard(initialTileIndex);
			whKnightPiece->addPieceBitBoard(newTileIndex);
		}
	}
	void makeMoveBlackPieceAI(const int& initialTileIndex, const int& newTileIndex) {
		if (((1ULL << initialTileIndex) & blKing) != 0) {
			timesBlackKingMoved++;
			blKingPiece->removePieceBitBoard(initialTileIndex);
			blKingPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & blPawn) != 0) {
			blPawnPiece->removePieceBitBoard(initialTileIndex);
			blPawnPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & blBishop) != 0) {
			blBishopPiece->removePieceBitBoard(initialTileIndex);
			blBishopPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & blRook) != 0) {
			blRookPiece->removePieceBitBoard(initialTileIndex);
			blRookPiece->addPieceBitBoard(newTileIndex);
			if ((blRook & 128) != 128) { //if right rook moved
				castleRooks[2] = -1;
				if (whenBlRightRookMoved == 0)
					whenBlRightRookMoved = blMoveCounter;
			}
			if ((blRook & 1) != 1) { //if left rook moved
				castleRooks[3] = -1;

				if (whenBlLeftRookMoved == 0)
					whenBlLeftRookMoved = blMoveCounter;
			}
		}
		else if (((1ULL << initialTileIndex) & blQueen) != 0) {
			blQueenPiece->removePieceBitBoard(initialTileIndex);
			blQueenPiece->addPieceBitBoard(newTileIndex);
		}
		else if (((1ULL << initialTileIndex) & blKnight) != 0) {
			blKnightPiece->removePieceBitBoard(initialTileIndex);
			blKnightPiece->addPieceBitBoard(newTileIndex);
		}
	}
public:
	void makeAIMove(uint16_t beforeNAfterMove, bool isItWhiteMove, const int currDepth) {
		if (isItWhiteMove == true) {
			makeAIMoveWhite(beforeNAfterMove, currDepth);
			enPassantWhite = 0ULL;
		}
		else {
			makeAIMoveBlack(beforeNAfterMove, currDepth);
			enPassantBlack = 0ULL;
		}
		isWhiteMove = !isWhiteMove;
	}
	void commitAIMove(uint16_t& beforeNAfterMove) {
		int dummyDepth = 0;

		if (isWhiteMove == true) {
			makeAIMoveWhite(beforeNAfterMove, dummyDepth);
			enPassantWhite = 0ULL;
		}
		else {
			makeAIMoveBlack(beforeNAfterMove, dummyDepth);
			enPassantBlack = 0ULL;
		}

		updateDisplayOfPieces();
		updateIsWhiteMove();
	}

	void addWhiteQueenToBoard(const int& index) { whQueenPiece->addPieceBitBoard(index); }
	void addWhiteRookToBoard(const int& index) { whRookPiece->addPieceBitBoard(index); }
	void addWhiteBishopToBoard(const int& index) { whBishopPiece->addPieceBitBoard(index); }
	void addWhiteKnightToBoard(const int& index) { whKnightPiece->addPieceBitBoard(index); }

	void addBlackQueenToBoard(const int& index) { blQueenPiece->addPieceBitBoard(index); }
	void addBlackRookToBoard(const int& index) { blRookPiece->addPieceBitBoard(index); }
	void addBlackBishopToBoard(const int& index) { blBishopPiece->addPieceBitBoard(index); }
	void addBlackKnightToBoard(const int& index) { blKnightPiece->addPieceBitBoard(index); }

	void removeCapturedWhitePiece(const int& index) {
		if (((1ULL << index) & whPawn) != 0) { whPawnPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & whBishop) != 0) { whBishopPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & whKnight) != 0) { whKnightPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & whRook) != 0) { whRookPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & whQueen) != 0) { whQueenPiece->removePieceBitBoard(index); }
		else
			throw std::runtime_error("Error in removing the captured White piece\n");
	}
	void removeCapturedBlackPiece(const int& index) {
		if (((1ULL << index) & blPawn) != 0) { blPawnPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & blBishop) != 0) { blBishopPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & blRook) != 0) { blRookPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & blQueen) != 0) { blQueenPiece->removePieceBitBoard(index); }
		else if (((1ULL << index) & blKnight) != 0) { blKnightPiece->removePieceBitBoard(index); }
		else
			throw std::runtime_error("Error in removing the captured Black piece\n");
	}
	void removeCapturedPiece(const int& index) {
		bitBoard &= ~(1ULL << index);
		try {
			if (((1ULL << index) & whPieces) != 0)
				removeCapturedWhitePiece(index);
			else
				removeCapturedBlackPiece(index);
		}
		catch (const std::runtime_error& e) {
			std::cout << e.what();
		}
		whPieces &= ~(1ULL << index);
		blPieces &= ~(1ULL << index);
		if (whMoveCounter < 5) {
			//customStartPosition();
		}
		updateDisplayOfPieces();
	}

	void updateBitBoard() {
		bitBoard = 0ULL;
		bitBoard |= whPawn;
		bitBoard |= whRook;
		bitBoard |= whBishop;
		bitBoard |= whKnight;
		bitBoard |= whKing;
		bitBoard |= whQueen;

		whPieces = 0ULL;
		whPieces |= bitBoard;

		bitBoard |= blPawn;
		bitBoard |= blRook;
		bitBoard |= blBishop;
		bitBoard |= blKnight;
		bitBoard |= blKing;
		bitBoard |= blQueen;

		blPieces = 0ULL;
		blPieces |= bitBoard ^ whPieces;
	}
	void updateDisplayOfPieces() {
		whKingPiece->updatePositionForDisplay();
		whPawnPiece->updatePositionForDisplay();
		whRookPiece->updatePositionForDisplay();
		whQueenPiece->updatePositionForDisplay();
		whBishopPiece->updatePositionForDisplay();
		whKnightPiece->updatePositionForDisplay();

		blKingPiece->updatePositionForDisplay();
		blPawnPiece->updatePositionForDisplay();
		blRookPiece->updatePositionForDisplay();
		blQueenPiece->updatePositionForDisplay();
		blBishopPiece->updatePositionForDisplay();
		blKnightPiece->updatePositionForDisplay();
	}
	void updateSquaresWhiteAttacks() {
		notCapturable = ~(whPieces | blKing);
		pinnedPiecesBitBoard = 0ULL;
		checkPathXRayThroughKing = 0ULL;
		enemyPiecesThatAreDefended = 0ULL;
		squaresToBlockCheckOrCapture = 0ULL;
		locationOfPieceAttackingKing = 0ULL;
		unsigned long long closestPiecesToKing = findClosestPieceToKing(numOfTrailingZeros(blKing)); //for finding pinned pieces for black

		whKingPiece->updateAttackSquares(whKing, whPieces);
		notCapturable ^= blKing;
		whPawnPiece->updateAttackSquaresWhite(whPawn, whPieces);
		whRookPiece->updateAttackSquares(whRook, blKing, closestPiecesToKing, whPieces);
		whQueenPiece->updateAttackSquares(whQueen, blKing, closestPiecesToKing, whPieces);
		whBishopPiece->updateAttackSquares(whBishop, blKing, closestPiecesToKing, whPieces);
		whKnightPiece->updateAttackSquares(whKnight, blKing, whPieces);
		notCapturable = ~(blPieces | whKing);

		squaresWhiteAttacks = 0ULL;
		squaresWhiteAttacks |= attackSquaresKing;
		squaresWhiteAttacks |= attackSquaresQueen;
		squaresWhiteAttacks |= attackSquaresRook;
		squaresWhiteAttacks |= attackSquaresBishop;
		squaresWhiteAttacks |= attackSquaresPawn;
		squaresWhiteAttacks |= attackSquaresKnight;

		blKingPiece->setSquaresTheEnemyAttacks(squaresWhiteAttacks);

		whAttackKing = 0ULL;
		whAttackRook = 0ULL;
		whAttackPawn = 0ULL;
		whAttackQueen = 0ULL;
		whAttackKnight = 0ULL;
		whAttackBishop = 0ULL;
		
		whAttackKing |= attackSquaresKing;
		whAttackQueen |= attackSquaresQueen;
		whAttackRook |= attackSquaresRook;
		whAttackBishop |= attackSquaresBishop;
		whAttackPawn |= attackSquaresPawn;
		whAttackKnight |= attackSquaresKnight;

		checkForCheckBlack();
	}
	void updateSquaresBlackAttacks() {
		notCapturable = ~(blPieces | whKing); //update with current board state
		pinnedPiecesBitBoard = 0ULL;
		checkPathXRayThroughKing = 0ULL;
		enemyPiecesThatAreDefended = 0ULL;
		squaresToBlockCheckOrCapture = 0ULL;
		locationOfPieceAttackingKing = 0ULL;
		unsigned long long closestPiecesToKing = findClosestPieceToKing(numOfTrailingZeros(whKing)); //for finding pinned pieces for white

		blKingPiece->updateAttackSquares(blKing, blPieces);
		notCapturable ^= whKing; //useful for finding checks
		blPawnPiece->updateAttackSquaresBlack(blPawn, blPieces);
		blRookPiece->updateAttackSquares(blRook, whKing, closestPiecesToKing, blPieces);
		blQueenPiece->updateAttackSquares(blQueen, whKing, closestPiecesToKing, blPieces);
		blBishopPiece->updateAttackSquares(blBishop, whKing, closestPiecesToKing, blPieces);
		blKnightPiece->updateAttackSquares(blKnight, whKing, blPieces);
		notCapturable = ~(whPieces | blKing); //For white pieces

		squaresBlackAttacks = 0ULL;
		squaresBlackAttacks |= attackSquaresKing;
		squaresBlackAttacks |= attackSquaresQueen;
		squaresBlackAttacks |= attackSquaresRook;
		squaresBlackAttacks |= attackSquaresBishop;
		squaresBlackAttacks |= attackSquaresPawn;
		squaresBlackAttacks |= attackSquaresKnight;

		whKingPiece->setSquaresTheEnemyAttacks(squaresBlackAttacks);

		blAttackKing = 0ULL;
		blAttackRook = 0ULL;
		blAttackPawn = 0ULL;
		blAttackQueen = 0ULL;
		blAttackKnight = 0ULL;
		blAttackBishop = 0ULL;

		blAttackKing |= attackSquaresKing;
		blAttackQueen |= attackSquaresQueen;
		blAttackRook |= attackSquaresRook;
		blAttackBishop |= attackSquaresBishop;
		blAttackPawn |= attackSquaresPawn;
		blAttackKnight |= attackSquaresKnight;

		checkForCheckWhite(); //check if white is now in check
	}
	void updateIsWhiteMove() { isWhiteMove = !isWhiteMove; }

	void updateWhenWhRightRookMoved() { whenWhRightRookMoved = whMoveCounter; }
	void updateWhenWhLeftRookMoved() { whenWhLeftRookMoved = whMoveCounter; }
	void updateWhenBlRightRookMoved() { whenBlRightRookMoved = blMoveCounter; }
	void updateWhenBlLeftRookMoved() { whenBlLeftRookMoved = blMoveCounter; }
	void updateTimesWhiteKingMoved() { timesWhiteKingMoved++; }
	void updateTimesBackKingMoved() { timesBlackKingMoved++; }
	 
	void updateWhMoveCounter() { whMoveCounter++; }
	void updateBlMoveCounter() { blMoveCounter++; }

	void checkForCheckWhite() {
		numberOfChecks = 0;
		
		if ((squaresBlackAttacks & whKing) == 0) {
			squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF;
			if (checkIfWhiteIsStaleMated() == true) 	{
				isStaleMate = true;
			}
			return;
		}

		if ((blAttackBishop & whKing) != 0) { numberOfChecks++; }
		if ((blAttackQueen & whKing) != 0) 	{ numberOfChecks++; }
		if ((blAttackRook & whKing) != 0) { numberOfChecks++; }
		if ((blAttackKnight & whKing) != 0) { numberOfChecks++; }
		if ((blAttackPawn & whKing) != 0) { numberOfChecks++; }

		if (numberOfChecks == 0) { squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF; }
		else if (checkIfWhiteIsMated() == true) { isCheckMate = true; }

	}
	void checkForCheckBlack() {
		numberOfChecks = 0;

		if ((squaresWhiteAttacks & blKing) == 0) {
			squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF;
			if (checkIfBlackIsStaleMated() == true) 	{
				isStaleMate = true;
			}
			return;
		}

		if ((whAttackBishop & blKing) != 0) { numberOfChecks++;	}
		if ((whAttackQueen & blKing) != 0) { numberOfChecks++; }
		if ((whAttackRook & blKing) != 0) { numberOfChecks++; }
		if ((whAttackKnight & blKing) != 0) { numberOfChecks++; }
		if ((whAttackPawn & blKing) != 0) { numberOfChecks++; }

		if (numberOfChecks == 0) { squaresToBlockCheckOrCapture = 0xFFFFFFFFFFFFFFFF; }
		else if (checkIfBlackIsMated() == true) { isCheckMate = true; }
	}
	
	bool checkIfWhiteIsMated() { return checkIfWhiteCannotMove(); }
	bool checkIfBlackIsMated() { return checkIfBlackCannotMove(); }
	bool checkIfWhiteIsStaleMated() { return checkIfWhiteCannotMove(); }
	bool checkIfBlackIsStaleMated() { return checkIfBlackCannotMove(); }
	bool checkIfWhiteCannotMove() {
		if (queenMovesWhite()->size() == 0)
			if (rookMovesWhite()->size() == 0)
				if (bishopMovesWhite()->size() == 0)
					if (knightMovesWhite()->size() == 0)
						if (pawnMovesWhite()->size() == 0)
							if (enPassantMovesWhite()->size() == 0)
								if (kingMovesWhite()->size() == 0)
									return true;
		return false;
	}
	bool checkIfBlackCannotMove() {
		if (queenMovesBlack()->size() == 0)
			if (rookMovesBlack()->size() == 0)
				if (bishopMovesBlack()->size() == 0)
					if (knightMovesBlack()->size() == 0)
						if (pawnMovesBlack()->size() == 0)
							if (enPassantMovesBlack()->size() == 0)
								if (kingMovesBlack()->size() == 0)
									return true;
		return false;
	}

	unsigned long long findClosestPieceToKing(const int& kingLocation) {
		return (HorzNVerticalMoves(kingLocation) | diagNAntiDagMoves(kingLocation) & notCapturable);
	}
	
	bool getIsWhiteMove() const { return isWhiteMove; }
	bool getIsCheckMate() const { return isCheckMate; }
	bool getIsStaleMate() const { return isStaleMate; }

	void setIsWhiteMove(const bool& wm) { isWhiteMove = wm; }
	void setIsCheckMate(const bool& cm) { isCheckMate = cm; }
	void setIsStaleMate(const bool& sm) { isStaleMate = sm; }

	int getWhenWhRightRookMoved() const { return whenWhRightRookMoved; }
	int getWhenWhLeftRookMoved() const { return whenWhLeftRookMoved; }

	int getWhenBlRightRookMoved() const { return whenBlRightRookMoved; }
	int getWhenBlLeftRookMoved() const { return whenBlLeftRookMoved; }
	
	Pawn* getWhitePawnPiece() { return whPawnPiece; }
	Rook* getWhiteRookPiece() { return whRookPiece; }
	Bishop* getWhiteBishopPiece() { return whBishopPiece; }
	Knight* getWhiteKnightPiece() { return whKnightPiece; }
	Queen* getWhiteQueenPiece() { return whQueenPiece; }
	King* getWhiteKingPiece() { return whKingPiece; }

	Pawn* getBlackPawnPiece() { return blPawnPiece; }
	Rook* getBlackRookPiece() { return blRookPiece; }
	Bishop* getBlackBishopPiece() { return blBishopPiece; }
	Knight* getBlackKnightPiece() { return blKnightPiece; }
	Queen* getBlackQueenPiece() { return blQueenPiece; }
	King* getBlackKingPiece() { return blKingPiece; }

	std::unique_ptr<std::vector<uint16_t>> kingMovesWhite() { return whKingPiece->legalMoves(whKing); }
	std::unique_ptr<std::vector<uint16_t>> queenMovesWhite() { return whQueenPiece->legalMoves(whQueen); }
	std::unique_ptr<std::vector<uint16_t>> rookMovesWhite() { return whRookPiece->legalMoves(whRook); }
	std::unique_ptr<std::vector<uint16_t>> bishopMovesWhite() { return whBishopPiece->legalMoves(whBishop); }
	std::unique_ptr<std::vector<uint16_t>> knightMovesWhite() { return whKnightPiece->legalMoves(whKnight); }
	std::unique_ptr<std::vector<uint16_t>> pawnMovesWhite() { return whPawnPiece->pawnMovesWhite(whPawn); }
	std::unique_ptr<std::vector<uint16_t>> castlingMovesWhite() { return whKingPiece->castlingMovesWhite(); }
	std::unique_ptr<std::vector<uint16_t>> enPassantMovesWhite() { return whPawnPiece->ePassantSquaresWhite(whPawn); }

	std::unique_ptr<std::vector<uint16_t>> kingMovesBlack() { return blKingPiece->legalMoves(blKing); }
	std::unique_ptr<std::vector<uint16_t>> queenMovesBlack() { return blQueenPiece->legalMoves(blQueen); }
	std::unique_ptr<std::vector<uint16_t>> rookMovesBlack() { return blRookPiece->legalMoves(blRook); }
	std::unique_ptr<std::vector<uint16_t>> bishopMovesBlack() { return blBishopPiece->legalMoves(blBishop); }
	std::unique_ptr<std::vector<uint16_t>> knightMovesBlack() { return blKnightPiece->legalMoves(blKnight); }
	std::unique_ptr<std::vector<uint16_t>> pawnMovesBlack() { return blPawnPiece->pawnMovesBlack(blPawn); }
	std::unique_ptr<std::vector<uint16_t>> castlingMovesBlack() { return blKingPiece->castlingMovesBlack(); }
	std::unique_ptr<std::vector<uint16_t>> enPassantMovesBlack() { return blPawnPiece->ePassantSquaresBlack(blPawn); }
};

#endif