#ifndef PIECE_H
#define PIECE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include "BoardInfo.h"

class Piece : protected BoardInfo{
protected:	
	int value;
	int numPieces = 0;
	float scale; //relative scale of the piece
	sf::Texture texture;
	sf::Sprite sprite;
	float dispX;
	float tileWidth;
	bool hasNotMoved = true;
	bool isWhite; //true for white piece, false for black piece
	bool didSpecialMove = false;
	std::string name;
	unsigned long long* bitBoardPosition = nullptr;
	unsigned long long positionForDisplay;
	unsigned long long squaresPieceAttacks;
	unsigned long long movableSquaresForDisplay;
	sf::RectangleShape attackingSquare;
	std::vector<unsigned char> pieceLocation;
	
	void fillPieceLocVector(unsigned long long pieceBitBoard) {
		unsigned long long piece = pieceBitBoard;
		while (piece != 0) {
			int location = numOfTrailingZeros(piece);
			pieceIndexBoard[location] = (unsigned char)pieceLocation.size();
			pieceLocation.push_back(location);
			pieceBitBoard &= ~(1ULL << location);
			piece = pieceBitBoard;
		}
	}
public:
	Piece() {
		value = 0;
		scale = 0;
		dispX = 0;
		tileWidth = 0;
		isWhite = false;
		bitBoardPosition = 0ULL;
		positionForDisplay = 0ULL;
		squaresPieceAttacks = 0ULL;
		movableSquaresForDisplay = 0ULL;
	}
	Piece(const int& val, const float& scal, const std::string& fileName, const float & x, const bool & isWhite, const std::string name, unsigned long long & bitBoardPosition, const int numPieces) {
		value = val;
		scale = scal;
		dispX = x;
		tileWidth = 2 * x;
		this->name = name;
		this->bitBoardPosition = &bitBoardPosition;
		this->isWhite = isWhite;
		this->numPieces = numPieces;
		positionForDisplay = bitBoardPosition;
		attackingSquare.setSize(sf::Vector2f(tileWidth, tileWidth));
		attackingSquare.setOrigin(sf::Vector2f(tileWidth / 2.f, tileWidth / 2.f));
		attackingSquare.setFillColor(sf::Color(172, 47, 255, 100));

		pieceLocation.reserve(8);
		fillPieceLocVector(bitBoardPosition);
		
		if (!setUpSprite(fileName)) {
			std::cout << "Failed loading image!" << std::endl;
		}
	}
	virtual ~Piece(){}
	bool setUpSprite(const std::string& fileName) {
		if (!texture.loadFromFile(fileName)) {
			return false;
		}

		sprite.setTexture(texture);
		sprite.setScale(sf::Vector2f(scale,scale));
		sprite.setOrigin(sf::Vector2f(333.f/2.f, 333.f/2.f));

		return true;
	}

	void drawPiece(sf::RenderWindow& window) {
		window.draw(sprite);
	}
	void drawPieceAttackSquares(sf::RenderWindow& window) {

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (std::bitset<1>(movableSquaresForDisplay >> (i * 8 + j)) == 1) {
					attackingSquare.setPosition(sf::Vector2f(dispX + tileWidth * j, dispX + tileWidth * i));
					window.draw(attackingSquare);
				}
			}
		}
	}

	void removeAPieceFromDisplay(const int& index) {
		//pieceLocation.erase(pieceLocation.begin() + pieceIndexBoard[index]);
		//pieceIndexBoard[index] = -1;
		positionForDisplay &= ~(1ULL << index);
	}
	void removePieceBitBoard(const int& index) {
		numPieces--;
		if (numPieces < 0) 	{
			std::cout << "Dang\n";
		}
		*bitBoardPosition &= ~(1ULL << index);
		//pieceLocation[pieceIndexBoard[index]] = -1;
		//pieceLocation.erase(pieceLocation.begin() + pieceIndexBoard[index]);
		//pieceIndexBoard[index] = -1;
	}
	//void removePieceVect(const int& index) {
	//	for (auto it = pieceLocation.begin(); it != pieceLocation.end(); it++) 		{
	//		if (*it == index) 	{
	//			pieceLocation.erase(it);
	//			break;
	//		}
	//	}
	//}

	void addPieceBitBoard(const int& index) {
		numPieces++;
		//pieceIndexBoard[index] = pieceLocation.size();
		//pieceLocation.push_back(index);
		*bitBoardPosition |= (1ULL << index);
	}
	void addAPieceToDisplay(const int& index) {
		//pieceIndexBoard[index] = pieceLocation.size();
		//pieceLocation.push_back(index);
		positionForDisplay |= (1ULL << index);
	}

	void setSpritePosition(sf::Vector2f position) {
		sprite.setPosition(position);
	}

	void updateBitBoardPosition() { 
		*bitBoardPosition = positionForDisplay;
	}
	void updatePositionForDisplay() { 
		positionForDisplay = *bitBoardPosition;
	}

	void printSprite(sf::RenderWindow& window) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (std::bitset<1>(positionForDisplay >> (i*8 + j)) == 1) {
					sprite.setPosition(dispX + tileWidth * j, dispX + tileWidth * i);
					window.draw(sprite);
				}
			}
		}
	}
	void printBitBoardPosition() {
		std::cout << "For the piece of value: " << value << std::endl;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (std::bitset<1>(positionForDisplay >> (i * 8 + j)) == 1) {
					std::cout << name << " ";
				} 
				
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	void printBitBoard(const unsigned long long& aBitBoard) { //for testing purposes to view the bitboard

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				std::cout << std::bitset<1>(aBitBoard >> (i * 8 + j)) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	sf::Vector2f getPosition() { return sprite.getPosition(); }
	sf::Sprite& getSprite() { return sprite; }
	std::string getName() const { return name; }
	int getValue() const { return value; }
	bool getIsWhite() const { return isWhite; }
	float getScale() const { return scale; }
	int getNumPieces() const{ return numPieces; }
	//std::vector<int> getPieceLocation() const { return pieceLocation; }
	int getLocVectSize() const { return (int)pieceLocation.size(); }
	void setNumPieces(const int& np) { numPieces = np; }
	void highLightAttackingSquares(sf::RenderWindow& window, unsigned long long& attackedSquares) {
		squaresPieceAttacks = attackedSquares;

		for (int i = 0; i < 8; i++) 		{
			for (int j = 0; j < 8; j++) 		{
				if (std::bitset<1>(squaresPieceAttacks >> (i * 8 + j)) == 1) {
					attackingSquare.setPosition(sf::Vector2f(dispX + tileWidth * j, dispX + tileWidth * i));
					window.draw(attackingSquare);
				}
			}
		}
	}

	virtual void findMoveableSquares(const int& piecePosition) {}
	virtual std::unique_ptr<std::vector<uint16_t>> playerLegalMoves() { return NULL; }
	virtual bool getHasNotMoved() { return hasNotMoved; }
	virtual bool getDidSpecialMove() { return false; }
};
#endif // !PIECE_H


//sf::Vector2f postion = entity.getPostion() //(x,y)
//can find the square by doing some calculation. do a modular of the position to find the square index number it goes to and then multiply by a constant to set the postion in the square