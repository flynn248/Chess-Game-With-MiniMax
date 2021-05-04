//#include "MoveGenerator.h"
#include "SFML/Graphics.hpp"
#include "ChessBoard.h"
#include <iostream>
#include <memory>
#ifndef PLAYER_H
#define PLAYER_H

class Player {
	std::shared_ptr<ChessBoard> board;
	//ChessBoard* board = nullptr;
	Piece* grabbedPiece = nullptr;
	bool madeEnPassantMove = false;
	bool madeCastleMove = false;
	bool pawnMovedTwoSquares = false;
	bool pawnPromoting = false;
	int rookBeforeCastle = -1;
	int rookAfterCastle = -1;
	//Have a vector of possible moves. When a piece is grabbed, update the vector which in turn updates the hilighted squares. Then when placed, check the vector if legal. Clear vector before grabbing another piece.
public:
	Player(std::shared_ptr<ChessBoard>& board) {
		this->board = board;
	}

	void movePiece(const int& x, const int& y, sf::RenderWindow& window) {
		int clickedTileIndex = y * 8 + x; //Finds the tile as the index
		sf::Sprite pieceSprite;
		grabbedPiece = nullptr;

		if ((board->bitBoard & (1ULL << clickedTileIndex)) == 0) { //if no piece was clicked
			return;
		}
		//std::cout << board->getBlackRookPiece()->getNumPieces() << " " << board->getWhiteRookPiece()->getNumPieces() << std::endl;
		if (board->getIsWhiteMove()) { //white to move
			if ((board->blPieces & (1ULL << clickedTileIndex)) != 0) { //if piece is not white
				return;
			}

			if (isThisPieceGrabbed(board->whPawn, clickedTileIndex)) {
				pieceSprite = board->getWhitePawnPiece()->getSprite();
				grabbedPiece = board->getWhitePawnPiece();

			}
			else if (isThisPieceGrabbed(board->whQueen, clickedTileIndex)) {
				pieceSprite = board->getWhiteQueenPiece()->getSprite();
				grabbedPiece = board->getWhiteQueenPiece();
			}
			else if (isThisPieceGrabbed(board->whBishop, clickedTileIndex)) {
				pieceSprite = board->getWhiteBishopPiece()->getSprite();
				grabbedPiece = board->getWhiteBishopPiece();
			}
			else if (isThisPieceGrabbed(board->whKnight, clickedTileIndex)) {
				pieceSprite = board->getWhiteKnightPiece()->getSprite();
				grabbedPiece = board->getWhiteKnightPiece();
			}
			else if (isThisPieceGrabbed(board->whRook, clickedTileIndex)) {
				pieceSprite = board->getWhiteRookPiece()->getSprite();
				grabbedPiece = board->getWhiteRookPiece();
			}
			else if (isThisPieceGrabbed(board->whKing, clickedTileIndex)) {
				pieceSprite = board->getWhiteKingPiece()->getSprite();
				grabbedPiece = board->getWhiteKingPiece();
			}
		}
		else { //black turn to move
			if ((board->whPieces & (1ULL << clickedTileIndex)) != 0) { //if piece is white
				return;
			}

			if (isThisPieceGrabbed(board->blPawn, clickedTileIndex)) {
				pieceSprite = board->getBlackPawnPiece()->getSprite();
				grabbedPiece = board->getBlackPawnPiece();
			}
			else if (isThisPieceGrabbed(board->blQueen, clickedTileIndex)) {
				pieceSprite = board->getBlackQueenPiece()->getSprite();
				grabbedPiece = board->getBlackQueenPiece();
			}
			else if (isThisPieceGrabbed(board->blBishop, clickedTileIndex)) {
				pieceSprite = board->getBlackBishopPiece()->getSprite();
				grabbedPiece = board->getBlackBishopPiece();
			}
			else if (isThisPieceGrabbed(board->blKnight, clickedTileIndex)) {
				pieceSprite = board->getBlackKnightPiece()->getSprite();
				grabbedPiece = board->getBlackKnightPiece();
			}
			else if (isThisPieceGrabbed(board->blRook, clickedTileIndex)) {
				pieceSprite = board->getBlackRookPiece()->getSprite();
				grabbedPiece = board->getBlackRookPiece();
			}
			else if (isThisPieceGrabbed(board->blKing, clickedTileIndex)) {
				pieceSprite = board->getBlackKingPiece()->getSprite();
				grabbedPiece = board->getBlackKingPiece();
			}
		}

		if (grabbedPiece->getIsWhite()) {
			board->notCapturable = ~(board->whPieces | board->blKing); //avoid illegal capture of black king
		}
		else {
			board->notCapturable = ~(board->blPieces | board->whKing); //avoid illegal capture of white king
		}

		bool isPieceGrabbed = true;
		int newX, newY;
		bool isLegalMove = false;
		grabbedPiece->removeAPieceFromDisplay(clickedTileIndex);

		while (isPieceGrabbed) {
			sf::Event e;

			while (window.pollEvent(e)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				pieceSprite.setPosition(sf::Vector2f((float)mousePos.x, (float)mousePos.y));
				grabbedPiece->findMoveableSquares(clickedTileIndex);

				if (e.type == sf::Event::MouseButtonReleased) {

					newX = (mousePos.x) / 120;
					newY = (mousePos.y) / 120;

					pieceSprite.setPosition(sf::Vector2f((float)(newX * 120 + 60), (float)(newY * 120 + 60)));

					if (newX != x || newY != y) { //if placed on a different square
						int newTileIndex = (newY * 8) + newX;

						if (isThisALegalSquare(newTileIndex, clickedTileIndex)) {

							if (isAnEnemyHere(newTileIndex)) { //if captured a piece
								if (pawnPromoting) {
									doEnemyPieceCapture(newTileIndex, clickedTileIndex);
									doPawnPromotion(newTileIndex, clickedTileIndex, window);
								}
								else
									doEnemyPieceCapture(newTileIndex, clickedTileIndex);
							}
							else if (pawnMovedTwoSquares) {
								doPawnMovedTwoSquares(newTileIndex);
							}
							else if (pawnPromoting)
								doPawnPromotion(newTileIndex, clickedTileIndex, window);
							else if (madeEnPassantMove)
								doEnpassantMove(newTileIndex, clickedTileIndex);
							else if (madeCastleMove)
								doCastleMove(newTileIndex, clickedTileIndex);
							else // Normal move
								grabbedPiece->addAPieceToDisplay(newTileIndex);
															
							
							updateGameState(clickedTileIndex, newTileIndex);
						}
						else //if not placed on legal square
							grabbedPiece->addAPieceToDisplay(clickedTileIndex);
					}
					else { //if placed on same square
						grabbedPiece->addAPieceToDisplay(clickedTileIndex);
					}
					isPieceGrabbed = false;
				}
				window.clear();
				board->drawBoard(window);
				grabbedPiece->drawPieceAttackSquares(window);
				window.draw(pieceSprite);
				window.display();
			}
		}
	}
private:
	void doEnemyPieceCapture(const int& newTileIndex, const int& clickedTileIndex) {
		board->removeCapturedPiece(newTileIndex);
		grabbedPiece->removeAPieceFromDisplay(clickedTileIndex);
		grabbedPiece->addAPieceToDisplay(newTileIndex);
	}
	void doPawnPromotion(const int& newTileIndex, const int& clickedTileIndex, sf::RenderWindow& window) {
		//do something to do with promoting a pawn
		grabbedPiece->removeAPieceFromDisplay(newTileIndex);
		
		int choice = -1;
		//Get a choice from user
		choice = displayPawnPromotionOptions(newTileIndex, window); //change this later
		if (board->getIsWhiteMove()) 	{
			if (choice == 0)
				board->addWhiteKnightToBoard(newTileIndex);
			else if (choice == 1)
				board->addWhiteBishopToBoard(newTileIndex);
			else if (choice == 2)
				board->addWhiteRookToBoard(newTileIndex);
			else
				board->addWhiteQueenToBoard(newTileIndex);
		}
		else 	{
			if (choice == 0)
				board->addBlackKnightToBoard(newTileIndex);
			else if (choice == 1)
				board->addBlackBishopToBoard(newTileIndex);
			else if (choice == 2)
				board->addBlackRookToBoard(newTileIndex);
			else
				board->addBlackQueenToBoard(newTileIndex);
		}
		board->removeCapturedPiece(clickedTileIndex);
	}
	void doEnpassantMove(const int& newTileIndex, const int& clickedTileIndex) {
		if (grabbedPiece->getIsWhite()) {
			board->removeCapturedPiece(newTileIndex + 8);
			grabbedPiece->removeAPieceFromDisplay(clickedTileIndex);
			grabbedPiece->addAPieceToDisplay(newTileIndex);
		}
		else {
			board->removeCapturedPiece(newTileIndex - 8);
			grabbedPiece->removeAPieceFromDisplay(clickedTileIndex);
			grabbedPiece->addAPieceToDisplay(newTileIndex);
		}
	}
	void doPawnMovedTwoSquares(const int& newTileIndex) {
		grabbedPiece->addAPieceToDisplay(newTileIndex);
		
		if (board->getIsWhiteMove())
			board->enPassantBlack |= (1ULL << (newTileIndex + 8));
		else
			board->enPassantWhite |= (1ULL << (newTileIndex - 8));
	}
	void doCastleMove(const int& newTileIndex, const int& clickedTileIndex) {
		//right and left rook are always relative to the white player
		
		grabbedPiece->addAPieceToDisplay(newTileIndex);
		//grabbedPiece->removePieceBitBoard(clickedTileIndex);
		//grabbedPiece->addPieceBitBoard(newTileIndex);
		grabbedPiece->updateBitBoardPosition();
		
		board->updateBitBoard();

		if (board->getIsWhiteMove()) 	{
			if (rookBeforeCastle == board->castleRooks[0])
				board->castleRooks[0] = -1;
			else
				board->castleRooks[1] = -1;
			board->addWhiteRookToBoard(rookAfterCastle);
		}
		else 	{
			if (rookBeforeCastle == board->castleRooks[2])
				board->castleRooks[2] = -1;
			else
				board->castleRooks[3] = -1;
			board->addBlackRookToBoard(rookAfterCastle);
		}

		board->removeCapturedPiece(rookBeforeCastle);
		board->updateDisplayOfPieces();
	}

	int displayPawnPromotionOptions(const int& newTileIndex, sf::RenderWindow& window) {
		sf::Texture knightTexture, bishopTexture, rookTexture, queenTexture;
		sf::Sprite knightSprite, bishopSprite, rookSprite, queenSprite;

		sf::Vector2f position;
		position.x = (float)((newTileIndex % 8) * 120 + 30);
		position.y = 30.f;

		if (board->getIsWhiteMove()) {
			setUpSpriteForPawnPromotion("assets/white-knight.png", knightTexture, knightSprite, position);
			position.x += 60.f;
			setUpSpriteForPawnPromotion("assets/white-bishop.png", bishopTexture, bishopSprite, position);
			position.x -= 60.f;
			position.y += 60.f;
			setUpSpriteForPawnPromotion("assets/white-rook.png", rookTexture, rookSprite, position);
			position.x += 60.f;
			setUpSpriteForPawnPromotion("assets/white-queen.png", queenTexture, queenSprite, position);
		}
		else {
			position.y = 870.f;
			setUpSpriteForPawnPromotion("assets/black-knight.png", knightTexture, knightSprite, position);
			position.x += 60.f;
			setUpSpriteForPawnPromotion("assets/black-bishop.png", bishopTexture, bishopSprite, position);
			position.x -= 60.f;
			position.y += 60.f;
			setUpSpriteForPawnPromotion("assets/black-rook.png", rookTexture, rookSprite, position);
			position.x += 60.f;
			setUpSpriteForPawnPromotion("assets/black-queen.png", queenTexture, queenSprite, position);
		}

		bool notPickedPiece = true;
		while (notPickedPiece) 	{
			sf::Event e;
			while (window.pollEvent(e)) 	{
				if (e.type == sf::Event::MouseButtonPressed) 	{
					if (e.mouseButton.button == sf::Mouse::Left) 	{
						if ((e.mouseButton.x / 120) != (newTileIndex % 8)) //if clicked somewhere other than the tile with the pieces on it
							break;
						if (e.mouseButton.y / 120 != ((newTileIndex - e.mouseButton.x / 120) / 8))
							break;
						

						int x = e.mouseButton.x / 60;
						int y = e.mouseButton.y / 60;

						if ((x % 2) == 0) 	{
							if ((y % 2) == 0)
								return 0; //chose knight
							else
								return 2; //chose rook
						}
						else 	{
							if ((y % 2) == 0)
								return 1; //chose bishop
							else
								return 3; //chose queen
						}
						notPickedPiece = false;
					}
				}
				window.clear();
				board->drawBoard(window);
				window.draw(knightSprite);
				window.draw(bishopSprite);
				window.draw(rookSprite);
				window.draw(queenSprite);
				window.display();
			}
		}

		return -1;
	}
	void setUpSpriteForPawnPromotion(const std::string& fileName, sf::Texture& texture, sf::Sprite& sprite, sf::Vector2f& position) {
		if (!texture.loadFromFile(fileName)) 	{
			std::cout << "Failed to load image for pawn promotion!" << std::endl;
			return;
		}

		sprite.setTexture(texture);
		sprite.setScale(sf::Vector2f(0.175f, 0.175f));
		sprite.setOrigin(sf::Vector2f(333.f / 2.f, 333.f / 2.f));
		sprite.setPosition(position);
	}

	void removeCapturedPiece(const int& index) {
		board->removeCapturedPiece(index);
	}

	bool isThisPieceGrabbed(const unsigned long long& pieceBitBoard, const int& clickedTileIndex) {
		
		if ((pieceBitBoard & (1ULL << clickedTileIndex)) != 0)
			return true;

		return false;
	}
	bool isAnEnemyHere(const int& newTileIndex) {
		if ((board->whPieces & (1ULL << newTileIndex)) != 0)
			return true;
		else if ((board->blPieces & (1ULL << newTileIndex)) != 0)
			return true;

		return false;
	}
	bool isThisALegalSquare(const int& newTileIndex, const int& initialTileIndex) { //maybe change this to make the legal move too
		std::unique_ptr<std::vector<uint16_t>> possibleMoves = std::make_unique<std::vector<uint16_t>>();
		rookBeforeCastle = -1;
		rookAfterCastle = -1;
		madeCastleMove = false; //bit 15
		madeEnPassantMove = false; //bit 14
		pawnMovedTwoSquares = false; //bit 7
		pawnPromoting = false; //bit 6

		possibleMoves = grabbedPiece->playerLegalMoves();
		uint16_t move;
		for (auto i = possibleMoves->begin(); i != possibleMoves->end(); i++) {
			move = *i;

			if ((move & 49344) == 0) 	{ //if no special moves
				if (((move >> 8) == newTileIndex) && ((move & 63) == initialTileIndex)) //if new moved square is equal to the tile the piece was placed
					return true;
			}
			else if((move & 64) == 64){ //Pawn promotion
				move &= 16191; //remove indicator bit
				if (((move >> 8) == newTileIndex) && ((move & 63) == initialTileIndex)) {
					pawnPromoting = true;
					return true;
				}
			}
			else if ((move & 128) == 128) 	{ //pawn moved two squares
				move &= 16191; 
				if (((move >> 8) == newTileIndex) && ((move & 63) == initialTileIndex)) {
					pawnMovedTwoSquares = true;
					return true;
				}
			}
			else if ((move & 16384) == 16384) { //if enpassant is possible
				move &= 16191; 
				if (((move >> 8) == newTileIndex) && ((move & 63) == initialTileIndex)) {
					madeEnPassantMove = true;
					return true;
				}
			}
			else{ //castling
				move &= 16191;
				if (((move >> 8) == (newTileIndex - 1)) || ((move >> 8) == (newTileIndex + 1))) {
					madeCastleMove = true;
					rookBeforeCastle = (move & 63);
					rookAfterCastle = (move & 16128) >> 8;
					return true;
				}
			}
		}

		return false;
	}

	void updateGameState(const int& initialTileIndex, const int& newTileIndex) {
		//grabbedPiece->removePieceBitBoard(initialTileIndex);
		//grabbedPiece->addPieceBitBoard(newTileIndex);
		grabbedPiece->updateBitBoardPosition();
		board->updateBitBoard();
		//board->printPiecesCount();
		//board->printPiecesLocVectSize();
		if (board->getIsWhiteMove()) 	{
			board->updateSquaresWhiteAttacks();
			board->enPassantWhite = 0ULL;
			
			if ((board->whRook & 0x8000000000000000) != 0x8000000000000000) { //if right rook moved
				board->castleRooks[0] = -1;
				if (board->getWhenWhRightRookMoved() != 0)
					board->updateWhenWhRightRookMoved();
			}
			if ((board->whRook & 0x100000000000000) != 0x100000000000000) { //if left rook moved
				board->castleRooks[1] = -1;
				if (board->getWhenWhLeftRookMoved() != 0)
					board->updateWhenWhLeftRookMoved();
			}
			if (King *king = dynamic_cast<King*>(grabbedPiece)) 	{
				king->updateHasKingMoved();
				board->updateTimesWhiteKingMoved();
			}
			board->updateWhMoveCounter();
		}
		else 	{
			board->updateSquaresBlackAttacks();
			board->enPassantBlack = 0ULL;

			if ((board->blRook & 128) != 128) { //if right rook moved
				board->castleRooks[2] = -1;
				if (board->getWhenBlRightRookMoved() != 0)
					board->updateWhenBlRightRookMoved();
			}
			if ((board->blRook & 1) != 1) { //if left rook moved
				board->castleRooks[3] = -1;

				if (board->getWhenBlLeftRookMoved() != 0)
					board->updateWhenBlLeftRookMoved();
			}
			if (King* king = dynamic_cast<King*>(grabbedPiece)) {
				king->updateHasKingMoved();
				board->updateTimesBackKingMoved();
			}
			board->updateBlMoveCounter();
		}
		board->updateIsWhiteMove(); //Change move to other player
	}

};

#endif